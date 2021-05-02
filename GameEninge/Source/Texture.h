#pragma once

#include <string>

#include "Arena.h"

struct Texture {
  unsigned char *buffer;
  unsigned int id;
  int width;
  int height;
  int bpp;
};

Texture *GetTexture(Arena *arena, const std::string& file_name);
void TextureBind(Texture *texture, unsigned int slot = 0);
void TextureUnbind();
void TextureFree(Texture *texture);
