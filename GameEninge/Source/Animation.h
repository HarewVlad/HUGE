#pragma once

#include <vector>
#include <boost\filesystem.hpp>

#include "Texture.h"
#include "Arena.h"

namespace bfs = boost::filesystem;

#define MAX_TEXTURES_PER_ANIMATION 32

struct Animation {
  Texture *textures[MAX_TEXTURES_PER_ANIMATION];
  Texture *current_texture;
  int textures_size;
};

Animation *GetAnimation(Arena *arena);
void AnimationAddAllFramesFromDirectory(Animation *animation, Arena *arena, const std::string& directory);
void AnimationUpdate(Animation *animation, float dt);
void AnimationFree(Animation *animation);
void AnimationBind(Animation *animation);