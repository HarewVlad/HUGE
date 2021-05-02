#include "Game.h"
#include "ImGuiManager.h"
#include <thread>
#include <chrono>

MakeInternal GLFWwindow *InitializeWindowAndGLFW(int width, int height, const std::string &title) {
  GLFWwindow *window = nullptr;

  /* Initialize the library */
  if (!glfwInit())
    ASSERT("glfwInit error!");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    ASSERT("GLFWwindow is nullptr!");
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  GLenum error = glewInit();
  if (error != GLEW_OK) {
    ASSERT("glewInit error!");
  }

  return window;
}

MakeInternal Game *GetGame(Arena *arena, int width, int height, const std::string &title) {
  Game *game = (Game *)ArenaAlloc(arena, sizeof(Game));

  game->window = InitializeWindowAndGLFW(width, height, title);
  game->grid = GetGrid(arena, width, height, Global_Grid_Amount);
  return game;
}

MakeInternal void GameUpdate(Game *game, float dt) {
  InputUpdate(game->input, dt);

  game->world->Step(dt, Global_Velocity_Iterations, Global_Position_Iterations);

  for (int i = 0; i < game->objects_size; ++i) {
    ObjectUpdate(game->objects[i], dt);
  }

  if (game->object_on_cursor != nullptr) {
    ObjectUpdate(game->object_on_cursor, dt);
  }

  CameraUpdate(game->camera, dt);
}

MakeInternal void RenderObjects(Game *game) {
  for (int i = 0; i < game->objects_size; ++i) {
    RendererDrawObject(game->renderer, game->camera, game->objects[i], game->programs[0]);
    if (game->is_debug_mode) {
      if (game->objects[i]->body != nullptr) {
        RendererDrawBody(game->renderer, game->camera, game->objects[i]->body, game->programs[1]);
      }
    }
    if (game->is_enable_grid) {
      RendererDrawGrid(game->renderer, game->grid, game->programs[1]);
    }
  }

  if (game->object_on_cursor != nullptr) {
    RendererDrawObject(game->renderer, game->camera, game->object_on_cursor, game->programs[0]);
  }
}

MakeInternal void GameRender(Game *game) {
  RendererClear(0.1f, 0.1f, 0.1f);
  RendererSetBlendState();

  RenderObjects(game);
  ImGuiManagerRender(game->imgui_manager, game);

  glfwSwapBuffers(game->window);
  glfwPollEvents();
}

MakeInternal void GameRun(Game *game) {
  float lastFrameTime = 0.0f;
  while (!glfwWindowShouldClose(game->window))
  {
    float time = static_cast<float>(glfwGetTime());
    float timestep = time - lastFrameTime;
    lastFrameTime = time;

    GameUpdate(game, timestep);
    GameRender(game);

    glfwPollEvents();
  }
}

MakeInternal void GameAddObject(Game *game, Object *object) {
  if (game->objects_size != GAME_MAX_OBJECTS) {
    game->objects[game->objects_size] = object;
    game->objects_size++;
  }
}

MakeInternal void GameAddProgram(Game *game, Program *program) {
  if (game->programs_size != GAME_MAX_PROGRAMS) {
    game->programs[game->programs_size] = program;
    game->programs_size++;
  }
}

MakeInternal void GameCreateSimpleProgram(Game *game, Arena *arena, const std::string& vs_source, const std::string& fs_source) {
  Program *program = GetProgram(arena);
  ProgramAddShader(program, GL_VERTEX_SHADER, vs_source);
  ProgramAddShader(program, GL_FRAGMENT_SHADER, fs_source);
  ProgramLink(program);
  GameAddProgram(game, program);
}

MakeInternal void GameFreeObjects(Game *game) {
  for (int i = 0; i < game->objects_size; ++i) {
    ObjectFree(game->objects[i]); // Free texture slots and such.
  }
  ArenaFree(&game->object_arena);
  memset(&game->objects[0], 0, game->objects_size * sizeof(Object *));
  game->objects_size = 0;
}

MakeInternal void GameCreateTestScene(Game *game) {
  Object *background = GetObject(&game->object_arena,
    Global_HWidth, Global_HHeight, "..\\Resources\\Background\\Sunny_Cloud_Background.png");
  GameAddObject(game, background);

  Object *player = GetObject(&game->object_arena, 300, 200,
    "..\\Resources\\Elf\\Idle\\Elf_M_Idle_1.png");
  Body *player_body = GetBody(&game->object_arena, game->world, b2_dynamicBody, 300, 200,
    player->texture->width,
    player->texture->height, 1.0f, 0.3f);
  player->body = player_body;
  ObjectCreateAddAnimation(player, ObjectState::IDLE, "..\\Resources\\Elf\\Idle");
  ObjectCreateAddAnimation(player, ObjectState::MOVE, "..\\Resources\\Elf\\Walk");
  GameAddObject(game, player);

  if (game->camera != nullptr) {
    CameraSetObjectThatFollows(game->camera, background);
    CameraSetObjectBased(game->camera, player);
  }

  Texture *texture = GetTexture(&game->object_arena, "..\\Resources\\Ground\\ground.png");
  for (int i = 0; i < 5; ++i) {
    Object *ground = GetObject(&game->object_arena,
      i * texture->width,
      texture->height * 0.5f,
      texture);
    Body *ground_body = GetBody(&game->object_arena, game->world, b2_staticBody,
      i * texture->width, texture->height * 0.5f,
      ground->texture->width,
      ground->texture->height, 1.0f, 0.3f);
    ground->body = ground_body;
    GameAddObject(game, ground);
  }

  Object *ground = GetObject(&game->object_arena, 200, 300, texture);
  Body *ground_body = GetBody(&game->object_arena, game->world, b2_staticBody,
    200, 300,
    ground->texture->width,
    ground->texture->height, 1.0f, 0.3f);
  ground->body = ground_body;
  GameAddObject(game, ground);
}

MakeInternal void GameSelectObjectOnCursor(Game *game, Texture *texture, bool is_create_body) {
  double x, y;
  glfwGetCursorPos(game->window, &x, &y);

  game->object_on_cursor = GetObject(&game->object_arena,
    static_cast<float>(x), static_cast<float>(y), texture);
  Body *body = GetBody(&game->object_arena, game->world, b2_staticBody,
    game->object_on_cursor->position.x, game->object_on_cursor->position.y,
    game->object_on_cursor->texture->width,
    game->object_on_cursor->texture->height, 1.0f, 0.3f);

  UtilsRunInDifferentThread([&]() { // Error prone multithreading.
    Game *game_temp = game;
    Body *body_temp = body;
    Texture *texture_temp = texture;
    bool is_create_body_temp = is_create_body;

    Object *object_on_cursor = game_temp->object_on_cursor;
    
    int state = glfwGetMouseButton(game_temp->window, GLFW_MOUSE_BUTTON_LEFT);
    while (state != GLFW_PRESS) {
      double x, y;
      glfwGetCursorPos(game_temp->window, &x, &y);

      object_on_cursor->position.x = static_cast<float>(x) + game_temp->camera->position.x;
      object_on_cursor->position.y = -static_cast<float>(y) + Global_Height + game_temp->camera->position.y;

      state = glfwGetMouseButton(game_temp->window, GLFW_MOUSE_BUTTON_LEFT);
    }

    if (is_create_body_temp) {
      while (game_temp->world->IsLocked()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      BodySetPosition(body_temp, b2Vec2(object_on_cursor->position.x, object_on_cursor->position.y));
      object_on_cursor->body = body_temp;
    }

    GameAddObject(game_temp, game_temp->object_on_cursor);
    game_temp->object_on_cursor = nullptr;
  });
}

MakeInternal void GameFree(Game *game) {
  glfwTerminate();
}