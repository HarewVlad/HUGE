#pragma once

#include "Object.h"
#include <glm/glm.hpp>

enum class CameraType {
  FREE,
  OBJECT_BASED
};

struct Camera {
  Object *object_to_track;
  Object *object_that_follows;
  glm::vec2 velocity;
  glm::mat4 view;
  glm::vec2 position;
  glm::vec2 target;
  CameraType type;
  /*float fade*/;
};

Camera GetCamera(CameraType type);
void CameraUpdate(Camera *camera, float dt);
void CameraSetObjectBased(Camera *camera, Object *object);
void CameraSetFree(Camera *camera);
void CameraMoveX(Camera *camera, float value);
void CameraMoveY(Camera *camera, float value);
void CameraSetObjectThatFollows(Camera *camera, Object *object);