#pragma once

#include "IndexBuffer.h"
#include "Utils.h"
#include "Arena.h"

struct IndexBuffer {
  unsigned int id;
  unsigned int count;
};

IndexBuffer *GetIndexBuffer(Arena *arena, const unsigned int *data, unsigned int count);
void IndexBufferBind(IndexBuffer *index_buffer);
void IndexBufferUnbind();
void IndexBufferFree(IndexBuffer *index_buffer);
