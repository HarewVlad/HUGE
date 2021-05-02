#pragma once

#include <vector>
#include <GL/glew.h>
#include "Utils.h"

struct VertexBufferElement {
  unsigned int type = 0;
  unsigned int count = 0;
  unsigned char normalized = 0;
};

unsigned int GetSizeOfOpenGLType(unsigned int type);

struct VertexBufferLayout {
  std::vector<VertexBufferElement> elements;
  unsigned int stride = 0;

  template <typename T>
  void Push(unsigned int count) {
    static_assert(false);
  }

  template <>
  void Push<float>(unsigned int count) {
    elements.push_back({ GL_FLOAT, count, GL_FALSE });
    stride += GetSizeOfOpenGLType(GL_FLOAT) * count;
  }

  template <>
  void Push<unsigned int>(unsigned int count) {
    elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    stride += GetSizeOfOpenGLType(GL_UNSIGNED_INT) * count;
  }

  template <>
  void Push<unsigned char>(unsigned int count) {
    elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    stride += GetSizeOfOpenGLType(GL_UNSIGNED_BYTE) * count;
  }
};