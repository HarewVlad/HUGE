#pragma once

#include "Arena.h"
#include "Texture.h"
#include "Game.h"

struct ImGuiManager {
  Arena texture_arena;

  Texture **textures = nullptr;
  Game *game = nullptr;

  bool is_create_body_for_object_on_cursor = false;
};

ImGuiManager GetImGuiManager(Game *game);
void ImGuiManagerRender(ImGuiManager *imgui_manager, Game *game);
void ImGuiManagerFree(ImGuiManager *imgui_manager);