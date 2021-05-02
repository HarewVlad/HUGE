#include <GL\glew.h>
#include "VertexBuffer.h"

MakeInternal VertexBuffer *GetVertexBuffer(Arena *arena, const void *data, unsigned int size) {
  VertexBuffer *vertex_buffer = (VertexBuffer *)ArenaAlloc(arena, sizeof(VertexBuffer));

  glCall(glGenBuffers(1, &vertex_buffer->id));
  glCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->id));
  glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

  return vertex_buffer;
}

MakeInternal void VertexBufferBindData(VertexBuffer *vertex_buffer, const void *data, unsigned int size) {
  glCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->id));
  glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

MakeInternal void VertexBufferBind(VertexBuffer* vertex_buffer) {
  glCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->id));
}

MakeInternal void VertexBufferUnbind() {
  glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

MakeInternal void VertexBufferFree(VertexBuffer* vertex_buffer) {
  glCall(glDeleteBuffers(1, &vertex_buffer->id));
}