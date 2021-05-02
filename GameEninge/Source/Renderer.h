#pragma once

#include <glm\glm.hpp>
#include "Object.h"
#include "Program.h"
#include "Grid.h"
#include "Camera.h"

struct Renderer {
  glm::mat4 view;
  glm::mat4 proj;
  glm::mat4 view_proj;
};

Renderer GetRenderer(int width, int height);
void RendererClear(float r, float g, float b);
void RendererSetBlendState();
void RendererDrawObject(Renderer *renderer, Camera *camera, Object *object, Program *program);
void RendererDrawBody(Renderer *renderer, Camera *camera, Body *body, Program *program);
void RendererDrawGrid(Renderer *renderer, Grid *grid, Program *program);