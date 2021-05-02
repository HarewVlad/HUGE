#pragma once

#include "Utils.h"
#include "Arena.h"

struct VertexBuffer {
  unsigned int id;
};

VertexBuffer *GetVertexBuffer(Arena *arena, const void *data, unsigned int size);
void VertexBufferBind(VertexBuffer* vertex_buffer);
void VertexBufferUnbind();
void VertexBufferFree(VertexBuffer* vertex_buffer);
void VertexBufferBindData(VertexBuffer *vertex_buffer, const void *data, unsigned int size);