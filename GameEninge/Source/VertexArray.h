#pragma once

#include "Utils.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

struct VertexArray {
  unsigned int id;
};

VertexArray *GetVertexArray(Arena *arena);
void VertexArrayAddBuffer(VertexArray *vertex_array,
  VertexBuffer *vertex_buffer,
  VertexBufferLayout *vertex_buffer_layout);
void VertexArrayBind(VertexArray *vertex_array);
void VertexArrayUnbind();
void VertexArrayFree(VertexArray *vertex_array);