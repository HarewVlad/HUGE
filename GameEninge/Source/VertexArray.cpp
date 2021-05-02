#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

MakeInternal VertexArray *GetVertexArray(Arena *arena) {
  VertexArray *vertex_array = (VertexArray *)ArenaAlloc(arena, sizeof(VertexArray));

  glCall(glGenVertexArrays(1, &vertex_array->id));

  return vertex_array;
}

MakeInternal void VertexArrayAddBuffer(VertexArray *vertex_array, 
  VertexBuffer *vertex_buffer, 
  VertexBufferLayout *vertex_buffer_layout) {
  glCall(glBindVertexArray(vertex_array->id));
  VertexBufferBind(vertex_buffer);

  const auto& elements = vertex_buffer_layout->elements;
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); ++i) {
    const auto& element = elements[i];

    glCall(glEnableVertexAttribArray(i));
    glCall(glVertexAttribPointer(i, element.count, element.type,
      element.normalized, vertex_buffer_layout->stride,
      (const void*)offset));
    offset += element.count * GetSizeOfOpenGLType(element.type);
  }
}

MakeInternal void VertexArrayBind(VertexArray *vertex_array) {
  glCall(glBindVertexArray(vertex_array->id));
}

MakeInternal void VertexArrayUnbind() {
  glCall(glBindVertexArray(0));
}

MakeInternal void VertexArrayFree(VertexArray *vertex_array) {
  glCall(glDeleteVertexArrays(1, &vertex_array->id));
}