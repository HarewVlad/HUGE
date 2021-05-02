#include "ImGuiManager.h"
#include "Utils.h"
#include "Vendor\imgui.h"
#include "Vendor\imgui_impl_glfw.h"
#include "Vendor\imgui_impl_opengl3.h"
#include "Vendor\stretchy_buffer.h"

#include <thread>
#include <iostream>

MakeInternal ImGuiManager GetImGuiManager(Game *game) {
  ImGuiManager imgui_manager;

  imgui_manager.game = game;

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(game->window, true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsDark();

  return imgui_manager;
}

MakeInternal void ImGuiManagerRender(ImGuiManager *imgui_manager, Game *game) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Hello, world!");
  {
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("Enable debug mode", &game->is_debug_mode);
    ImGui::Checkbox("Enable grid", &game->is_enable_grid);

    if (ImGui::Button("Free camera")) {
      CameraSetFree(game->camera);
    }

    if (ImGui::Button("Select objects")) {
      if (sb_count(imgui_manager->textures) != 0) {
        sb_free(imgui_manager->textures);
        imgui_manager->textures = nullptr;
      }
      game->is_load_textures_for_selection = true;
    }

    ImGui::SameLine();
    ImGui::Checkbox("Make body for object on cursor?", &imgui_manager->is_create_body_for_object_on_cursor);
    ImGui::NewLine();

    if (game->is_load_textures_for_selection) {
      /* Folder must contain all the images to select. */
      std::string folder_name = UtilsSelectFolder();

      if (!folder_name.empty()) {
        bfs::directory_iterator end_itr;
        for (bfs::directory_iterator itr(folder_name); itr != end_itr; ++itr) {
          if (bfs::is_directory(itr->status())) {
            continue;
          }
          else {
            auto path = itr->path();

            Texture *texture = GetTexture(&game->texture_arena, path.string());
            sb_push(imgui_manager->textures, texture);
          }
        }
      }

      game->is_load_textures_for_selection = false;
    }

    ImGui::BeginChild("Images");
    {
      for (int i = 0; i < sb_count(imgui_manager->textures); ++i) {
        ImGui::SameLine();
        if (ImGui::ImageButton((ImTextureID)imgui_manager->textures[i]->id, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0))) {
          GameSelectObjectOnCursor(imgui_manager->game, imgui_manager->textures[i], 
            imgui_manager->is_create_body_for_object_on_cursor);
        }
        if (i % 2 == 0 && i != 0) {
          ImGui::NewLine();
        }
      }
    }
    ImGui::EndChild();
  }
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

MakeInternal void ImGuiManagerFree(ImGuiManager *imgui_manager) {
  if (sb_count(imgui_manager->textures) != 0) {
    for (int i = 0; i < sb_count(imgui_manager->textures); ++i) {
      TextureFree(imgui_manager->textures[i]);
    }
    ArenaFree(&imgui_manager->texture_arena);
  }
}