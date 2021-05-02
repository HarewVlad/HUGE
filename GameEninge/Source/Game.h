#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <box2d\box2d.h>

#include "Utils.h"
#include "Config.h"
#include "Object.h"
#include "Input.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Utils.h"
#include "Arena.h"
#include "Renderer.h"
#include "Grid.h"
#include "Camera.h"
#include "Vendor\imgui.h"
#include "Vendor\imgui_impl_glfw.h"
#include "Vendor\imgui_impl_opengl3.h"
#include "Vendor\stretchy_buffer.h"

#define GAME_MAX_OBJECTS 256
#define GAME_MAX_PROGRAMS 64

struct ImGuiManager;

struct Game {
  Arena object_arena;
  Arena animation_arena;
  Arena texture_arena; // For texture selection.

  GLFWwindow *window;
  Renderer *renderer;
  b2World *world;
  Input *input;
  ImGuiManager *imgui_manager;
  Grid *grid;
  Camera *camera;
  Object *object_on_cursor;
  Object *objects[GAME_MAX_OBJECTS];
  Program *programs[GAME_MAX_PROGRAMS];
  int objects_size;
  int programs_size;

  bool is_debug_mode;
  bool is_enable_grid;
  bool is_load_textures_for_selection;
};

Game *GetGame(Arena *arena, int width, int height, const std::string &title);
void GameUpdate(Game *game, float dt);
void GameRun(Game *game);
void GameAddObject(Game *game, Object *object);
void GameAddProgram(Game *game, Program *program);
void GameCreateSimpleProgram(Game *game, Arena *arena, const std::string& vs_source, const std::string& fs_source);
void GameFreeObjects(Game *game);
void GameCreateTestScene(Game *game);
void GameSelectObjectOnCursor(Game *game, Texture *texture, bool is_create_body);
void GameFree(Game *game);