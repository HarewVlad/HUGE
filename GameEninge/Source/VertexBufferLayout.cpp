#include "VertexBufferLayout.h"

MakeInternal unsigned int GetSizeOfOpenGLType(unsigned int type) {
  switch (type) {
  case GL_FLOAT:
    return 4;
  case GL_UNSIGNED_INT:
    return 4;
  case GL_UNSIGNED_BYTE:
    return 1;
  default:
    return -1;
  }
}