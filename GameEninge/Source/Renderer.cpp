#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Renderer.h"
#include "Utils.h"

#define DEBUG_DRAW

MakeInternal Renderer GetRenderer(int width, int height) {
  Renderer renderer;

  renderer.proj = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1000.0f, 1000.0f);

  return renderer;
}

MakeInternal void RendererClear(float r, float g, float b) {
  glCall(glClearColor(r, g, b, 1.0f));
  glCall(glClear(GL_COLOR_BUFFER_BIT));
}

MakeInternal void RendererSetBlendState() {
  glCall(glEnable(GL_BLEND));
  glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

MakeInternal void RendererDrawObject(Renderer *renderer, Camera *camera, Object *object, Program *program) {
  ProgramBind(program);

  ObjectBind(object);
  ProgramSetUniformMat4(program, "u_MVP", renderer->proj * camera->view * ObjectGetModel(object));
  ProgramSetUniform1i(program, "u_Texture", 0);
  glCall(glDrawElements(GL_TRIANGLES, object->index_buffer->count, GL_UNSIGNED_INT, nullptr));
  ObjectUnbind();

  ProgramUnbind();
}

MakeInternal void RendererDrawBody(Renderer *renderer, Camera *camera, Body *body, Program *program) {
  ProgramBind(program);

  BodyBind(body);
  ProgramSetUniformMat4(program, "u_MVP", renderer->proj * camera->view * BodyGetModel(body));
  glCall(glDrawElements(GL_LINE_STRIP, body->index_buffer->count, GL_UNSIGNED_INT, nullptr));
  BodyUnbind();

  ProgramUnbind();
}

MakeInternal void RendererDrawGrid(Renderer *renderer, Grid *grid, Program *program) {
  ProgramBind(program);

  GridBind(grid);
  ProgramSetUniformMat4(program, "u_MVP", renderer->proj * grid->view * grid->model);
  glCall(glDrawElements(GL_LINES, grid->index_buffer->count, GL_UNSIGNED_INT, nullptr));
  GridUnbind();

  ProgramUnbind();
}