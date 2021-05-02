#include "Animation.h"
#include "Utils.h"

MakeInternal Animation *GetAnimation(Arena *arena) {
  Animation *animation = (Animation *)ArenaAlloc(arena, sizeof(Animation));

  return animation;
}

MakeInternal void AnimationAddAllFramesFromDirectory(Animation *animation, Arena *arena, const std::string& directory) {
  bfs::directory_iterator end_itr;
  for (bfs::directory_iterator itr(directory); itr != end_itr; ++itr) {
    if (bfs::is_directory(itr->status())) {
      continue;
    }
    else {
      auto path = itr->path();

      if (animation->textures_size != MAX_TEXTURES_PER_ANIMATION) {
        Texture *texture = GetTexture(arena, path.string());
        animation->textures[animation->textures_size] = texture;
        ++animation->textures_size;
      }
      else {
        // Not enough space to add new texture.
        return;
      }
    }
  }

  animation->current_texture = animation->textures[0];
}

MakeInternal void AnimationUpdate(Animation *animation, float dt) {
  static float time = 0.0f;
  static int index = 0;

  if (animation->textures_size == 1) return;

  time += dt;
  if (time >= 1 / 24.0f) {
    index = (index + 1) % animation->textures_size;
    animation->current_texture = animation->textures[index];
    time = 0.0f;
  }
}

MakeInternal void AnimationFree(Animation *animation) {
  // Need to free graphics texture slot.
  for (int i = 0; i < animation->textures_size; ++i) {
    TextureFree(animation->textures[i]);
  }
}

MakeInternal void AnimationBind(Animation *animation) {
  TextureBind(animation->current_texture);
}