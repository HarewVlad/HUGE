#include <GL\glew.h>
#include "Texture.h"
#include "Utils.h"
#include "Vendor\stb_image.h"

MakeInternal Texture *GetTexture(Arena *arena, const std::string& file_name) {
  Texture *texture = (Texture *)ArenaAlloc(arena, sizeof(Texture));

  stbi_set_flip_vertically_on_load(1);
  texture->buffer = stbi_load(file_name.c_str(), &texture->width, &texture->height, &texture->bpp, 4);

  glCall(glGenTextures(1, &texture->id));
  glCall(glBindTexture(GL_TEXTURE_2D, texture->id));

  glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->buffer));
  glCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (texture->buffer) {
    stbi_image_free(texture->buffer);
  }

  return texture;
}

MakeInternal void TextureBind(Texture *texture, unsigned int slot) {
  glCall(glActiveTexture(GL_TEXTURE0 + slot));
  glCall(glBindTexture(GL_TEXTURE_2D, texture->id));
}

MakeInternal void TextureUnbind() {
  glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

MakeInternal void TextureFree(Texture *texture) {
  glCall(glDeleteTextures(1, &texture->id));
}