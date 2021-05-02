// TODO: Make proper include of herader and realization files (*.cpp)
// TODO: Freeze world when alt-tabbed or something
// TODO: Add logger
// TODO: Remove the grid?

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <box2d\box2d.h>

#include "Main.h"
#include "Game.h"
#include "List.h"

#define CAMERA_SPEED 5.0f

MakeInternal void GameInput(Game *game, int key) {
  // TODO(vlad): Make proper selection of current player.
  Object *object = game->objects[1];
  switch (key) {
  case GLFW_KEY_A:
    BodyMove(object->body, { -5, 0 });
    object->current_object_state = ObjectState::MOVE;
    // ObjectRotateY(object, -5);
    ObjectFlipY(object, true);
    CameraMoveX(game->camera, -CAMERA_SPEED);
    break;
  case GLFW_KEY_S:
    BodyMove(object->body, { 0, -5 });
    object->current_object_state = ObjectState::MOVE;
    CameraMoveY(game->camera, -CAMERA_SPEED);
    break;
  case GLFW_KEY_D:
    BodyMove(object->body, { 5, 0 });
    object->current_object_state = ObjectState::MOVE;
    // ObjectRotateY(object, 5);
    ObjectFlipY(object, false);
    CameraMoveX(game->camera, CAMERA_SPEED);
    break;
  case GLFW_KEY_SPACE:
    BodyMove(object->body, { 0, 5 });
    object->current_object_state = ObjectState::MOVE;
    CameraMoveY(game->camera, CAMERA_SPEED);
    break;
  }
}

void TestList() {
  List list = GetList();

  static char *string = "XYZ";
  for (int i = 0; i < strlen(string); ++i) {
    ListPush(&list, &string[i]);
  }
  for (int i = 0; i < strlen(string); ++i) {
    ListPop(&list);
  }
  ASSERT(list.size == 0);

  for (int i = 0; i < strlen(string); ++i) {
    ListPush(&list, &string[i]);
  }
  for (int i = 0; i < strlen(string); ++i) {
    ListPopAt(&list, 0);
  }
  ASSERT(list.size == 0);
}

void Tests() {
  TestList();
}

int main() {
  Tests();

  Arena game_arena;

  Game *game = GetGame(&game_arena, Global_Width, Global_Height, Global_Title);
  defer{ GameFree(game); };

  Input input = GetInput(game->window, [&](int key) { GameInput(game, key); });
  InputAddKeyToTrack(&input, GLFW_KEY_A);
  InputAddKeyToTrack(&input, GLFW_KEY_S);
  InputAddKeyToTrack(&input, GLFW_KEY_D);
  InputAddKeyToTrack(&input, GLFW_KEY_SPACE);

  Camera camera = GetCamera(CameraType::FREE);

  Renderer renderer = GetRenderer(Global_Width, Global_Height);

  ImGuiManager imgui_manager = GetImGuiManager(game);
  defer{ ImGuiManagerFree(&imgui_manager); };

  b2World world(Global_Gravity);

  game->renderer = &renderer;
  game->world = &world;
  game->input = &input;
  game->imgui_manager = &imgui_manager;
  game->camera = &camera;

  GameCreateSimpleProgram(game, &game_arena,
    "Source\\Shaders\\vs.hlsl", "Source\\Shaders\\fs.hlsl");
  GameCreateSimpleProgram(game, &game_arena,
    "Source\\Shaders\\debug_vs.hlsl", "Source\\Shaders\\debug_fs.hlsl");

  GameCreateTestScene(game);

  GameRun(game);
}