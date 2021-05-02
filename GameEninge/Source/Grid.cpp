#include "Vendor/stretchy_buffer.h"
#include "Grid.h"
#include "Utils.h"

MakeInternal IndexBuffer *GridGetDefaultIndexBuffer(Arena *arena, int w, int h, int amount) {
  IndexBuffer *index_buffer = nullptr;

  unsigned int *indices = nullptr;
  for (int i = 0; i < amount; ++i) {
    for (int j = 0; j < amount; ++j) {
      unsigned int i0 = i * (amount + 1) + j;
      unsigned int i1 = (i + 1) * (amount + 1) + j;
      unsigned int i2 = (i + 1) * (amount + 1) + (j + 1);
      unsigned int i3 = i * (amount + 1) + (j + 1);

      sb_push(indices, i0);
      sb_push(indices, i1);
      sb_push(indices, i1);
      sb_push(indices, i2);
      sb_push(indices, i2);
      sb_push(indices, i3);
      sb_push(indices, i3);
      sb_push(indices, i0);
    }
  }

  index_buffer = GetIndexBuffer(arena, indices, sb_count(indices));

  sb_free(indices);

  return index_buffer;
}

MakeInternal VertexBuffer *GridGetDefaultVertexBuffer(Arena *arena, int w, int h, int amount) {
  VertexBuffer *vertex_buffer = nullptr;

  float *vertices = nullptr;
  float x_offset = w / (float)amount;
  float y_offset = h / (float)amount;
  for (int i = 0; i < amount + 1; ++i) {
    for (int j = 0; j < amount + 1; ++j) {
      float x = j * x_offset;
      float y = i * y_offset;

      sb_push(vertices, x);
      sb_push(vertices, y);
    }
  }

  vertex_buffer = GetVertexBuffer(arena, vertices, sizeof(float) * sb_count(vertices));

  sb_free(vertices);

  return vertex_buffer;
}

MakeInternal VertexArray *GridGetDefaultVertexArray(Arena *arena, VertexBuffer *vertex_buffer) {
  VertexArray *vertex_array = nullptr;

  VertexBufferLayout vbl;
  vbl.Push<float>(2);

  vertex_array = GetVertexArray(arena);
  VertexArrayAddBuffer(vertex_array, vertex_buffer, &vbl);

  return vertex_array;
}

MakeInternal Grid *GetGrid(Arena *arena, int w, int h, int amount) {
  Grid *grid = (Grid *)ArenaAlloc(arena, sizeof(Grid));

  grid->vertex_buffer = GridGetDefaultVertexBuffer(arena, w, h, amount);
  grid->index_buffer = GridGetDefaultIndexBuffer(arena, w, h, amount);
  grid->vertex_array = GridGetDefaultVertexArray(arena, grid->vertex_buffer);
  grid->model = glm::mat4(1.0f);
  grid->view = glm::mat4(1.0f);

  return grid;
}

MakeInternal void GridFree(Grid *grid) {
  VertexBufferFree(grid->vertex_buffer);
  IndexBufferFree(grid->index_buffer);
  VertexArrayFree(grid->vertex_array);
}

MakeInternal void GridBind(Grid *grid) {
  VertexArrayBind(grid->vertex_array);
  IndexBufferBind(grid->index_buffer);
}

MakeInternal void GridUnbind() {
  VertexArrayUnbind();
  IndexBufferUnbind();
}