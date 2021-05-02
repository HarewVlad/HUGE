#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Arena.h"

#include <glm/glm.hpp>

struct Grid {
  VertexArray *vertex_array;
  VertexBuffer *vertex_buffer;
  IndexBuffer *index_buffer;
  glm::mat4 view;
  glm::mat4 model;
};

Grid *GetGrid(Arena *arena, int w, int h, int amount);
void GridFree(Grid *grid);
void GridBind(Grid *grid);
void GridUnbind();