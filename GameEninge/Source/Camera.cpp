#include "Camera.h"
#include "Utils.h"
#include "Config.h"

#include <glm\gtx\transform.hpp>
#include <iostream>

MakeInternal Camera GetCamera(CameraType type) {
  Camera camera;

  camera.type = type;
  camera.object_to_track = nullptr;
  camera.object_that_follows = nullptr;
  camera.position = { 0, 0 };
  camera.velocity = { 0, 0 };
  camera.target = { 0, 0 };
  camera.view = glm::mat4(1.0f);

  return camera;
}

MakeInternal bool CameraUpdateObjectBased(Camera *camera, float dt) {
  bool result = false;

  if (camera->object_to_track != nullptr && camera->position != camera->object_to_track->position) {
    camera->position = camera->object_to_track->position;
    camera->target = camera->object_to_track->position;

    result = true;
  }

  return result;
}

MakeInternal bool CameraUpdateFree(Camera *camera, float dt) {
  if (camera->velocity.x == 0 && camera->velocity.y == 0) {
    return false;
  }

  camera->position += camera->velocity * dt;
  camera->target += camera->velocity * dt;

  camera->velocity -= camera->velocity * dt;

  return true;
}

MakeInternal void CameraUpdateObjectThatFollows(Camera *camera, float dt) {
  if (camera->object_that_follows != nullptr) {
    camera->object_that_follows->position = camera->position;
  }
}

MakeInternal void CameraUpdate(Camera *camera, float dt) {
  bool is_view_dirty = false;

  switch (camera->type) {
  case CameraType::OBJECT_BASED:
    is_view_dirty = CameraUpdateObjectBased(camera, dt);
    break;
  case CameraType::FREE:
    is_view_dirty = CameraUpdateFree(camera, dt);
    break;
  }

  CameraUpdateObjectThatFollows(camera, dt);

  if (is_view_dirty) {
    // Center the camera.
    if (camera->type == CameraType::OBJECT_BASED) {
      camera->position.x -= Global_HWidth;
      camera->position.y -= Global_HHeight;
      camera->target.x -= Global_HWidth;
      camera->target.y -= Global_HHeight;
    }
    else { // Center the following object.
      camera->object_that_follows->position.x += Global_HWidth;
      camera->object_that_follows->position.y += Global_HHeight;
    }

    camera->view = glm::lookAt(
      glm::vec3(camera->position, 0),
      glm::vec3(camera->target, -1),
      glm::vec3(0, 1, 0));
  }
}

MakeInternal void CameraSetObjectBased(Camera *camera, Object *object) {
  camera->type = CameraType::OBJECT_BASED;
  camera->object_to_track = object;
}

MakeInternal void CameraSetFree(Camera *camera) {
  camera->type = CameraType::FREE;
  camera->object_to_track = nullptr;
}

MakeInternal void CameraMoveX(Camera *camera, float value) {
  camera->velocity.x += value;
}

MakeInternal void CameraMoveY(Camera *camera, float value) {
  camera->velocity.y += value;
}

MakeInternal void CameraSetObjectThatFollows(Camera *camera, Object *object) {
  if (object->body != nullptr) {
    ASSERT("Object that follows has body!");
    return;
  }

  camera->object_that_follows = object;
}