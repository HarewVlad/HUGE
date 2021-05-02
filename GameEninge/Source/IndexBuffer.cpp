#include <GL\glew.h>
#include "IndexBuffer.h"

MakeInternal IndexBuffer *GetIndexBuffer(Arena *arena, const unsigned int *data, unsigned int count) {
  IndexBuffer *index_buffer = (IndexBuffer *)ArenaAlloc(arena, sizeof(IndexBuffer));

  index_buffer->count = count;

  glCall(glGenBuffers(1, &index_buffer->id));
  glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id));
  glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
    data, GL_STATIC_DRAW));

  return index_buffer;
}

MakeInternal void IndexBufferBind(IndexBuffer *index_buffer) {
  glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id));
}

MakeInternal void IndexBufferUnbind() {
  glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

MakeInternal void IndexBufferFree(IndexBuffer *index_buffer) {
  glCall(glDeleteBuffers(1, &index_buffer->id));
}