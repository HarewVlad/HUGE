#pragma once

#include <glm\glm.hpp>
#include <box2d\box2d.h>

#include "Arena.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Body.h"

// TODO: Ability to add as many states as u want.

#define OBJECT_MAX_ANIMATIONS 16

enum class ObjectState {
  IDLE = 0,
  MOVE,
  JUMP
};

struct Object {
  Arena animation_arena;

  VertexBuffer *vertex_buffer;
  IndexBuffer *index_buffer;
  VertexArray *vertex_array;
  Texture *texture;
  Animation *animations[OBJECT_MAX_ANIMATIONS];
  int animations_size;
  int animation_filled_indexes[OBJECT_MAX_ANIMATIONS];
  int animation_filled_indexes_size;
  Body *body;
  glm::vec2 position;
  float angle;
  ObjectState current_object_state;
};

Object *GetObject(Arena *arena, float x, float y, const std::string& source);
Object *GetObject(Arena *arena, float x, float y, Texture *texture);
void ObjectBind(Object *object);
void ObjectUnbind();
void ObjectUpdate(Object *object, float dt);
void ObjectFree(Object *object);
void ObjectAddAnimation(Object *object, ObjectState state, Animation *animation);
void ObjectFreeAnimations(Object *object);
void ObjectCreateAddAnimation(Object *object, ObjectState state, const std::string& directory);
void ObjectRotateY(Object *object, float angle);
void ObjectFlipY(Object *object, bool flag);
void ObjectTranslate(Object *object, float x, float y);
glm::mat4 ObjectGetModel(Object *object);