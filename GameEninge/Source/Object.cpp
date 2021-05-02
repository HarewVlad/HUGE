#include <iostream>

#include "Object.h"
#include "Utils.h"

#include <glm\gtx\transform.hpp>

MakeInternal VertexBuffer *ObjectCreateDefaultVertexBuffer(Arena *arena, int width, int height) {
  VertexBuffer *vertex_buffer = nullptr;

  std::vector<float> vertices = {
    -width / 2.0f, -height / 2.0f, 0.0f, 0.0f,
    -width / 2.0f, height / 2.0f, 0.0f, 1.0f,
    width / 2.0f, height / 2.0f, 1.0f, 1.0f,
    width / 2.0f, -height / 2.0f, 1.0f, 0.0f
  };

  vertex_buffer = GetVertexBuffer(arena, &vertices[0], sizeof(float) * vertices.size());

  return vertex_buffer;
}

MakeInternal IndexBuffer *ObjectCreateDefaultIndexBuffer(Arena *arena) {
  IndexBuffer *index_buffer = nullptr;

  std::vector<unsigned int> indices = {
    0, 1, 2,
    0, 2, 3
  };

  index_buffer = GetIndexBuffer(arena, &indices[0], indices.size());

  return index_buffer;
}

MakeInternal VertexArray *ObjectCreateDefaultVertexArray(Arena *arena, VertexBuffer *vertex_buffer) {
  VertexArray *vertex_array = nullptr;

  VertexBufferLayout vbl;
  vbl.Push<float>(2);
  vbl.Push<float>(2);

  vertex_array = GetVertexArray(arena);
  VertexArrayAddBuffer(vertex_array, vertex_buffer, &vbl);

  return vertex_array;
}

MakeInternal Object *GetObject(Arena *arena, float x, float y, const std::string& source) {
  Object *object = (Object *)ArenaAlloc(arena, sizeof(Object));

  object->current_object_state = ObjectState::IDLE;
  object->texture = GetTexture(arena, source);
  object->position = { x, y };
  object->index_buffer = ObjectCreateDefaultIndexBuffer(arena);
  object->vertex_buffer = ObjectCreateDefaultVertexBuffer(arena,  
    object->texture->width, 
    object->texture->height);
  object->vertex_array = ObjectCreateDefaultVertexArray(arena, object->vertex_buffer);

  return object;
}

MakeInternal Object *GetObject(Arena *arena, float x, float y, Texture *texture) {
  Object *object = (Object *)ArenaAlloc(arena, sizeof(Object));

  object->current_object_state = ObjectState::IDLE;
  object->texture = texture;
  object->position = { x, y };
  object->index_buffer = ObjectCreateDefaultIndexBuffer(arena);
  object->vertex_buffer = ObjectCreateDefaultVertexBuffer(arena,
    object->texture->width,
    object->texture->height);
  object->vertex_array = ObjectCreateDefaultVertexArray(arena, object->vertex_buffer);

  return object;
}

MakeInternal void ObjectBind(Object *object) {
  VertexArrayBind(object->vertex_array);
  IndexBufferBind(object->index_buffer);
  // We can eventually free animation, so need to check whether it's still alive.
  if (object->animations[(int)object->current_object_state] != nullptr) { // If we don't have animation, than bind main texture.
    AnimationBind(object->animations[(int)object->current_object_state]);
  } else {
    TextureBind(object->texture);
  }
}

MakeInternal void ObjectUnbind() {
  VertexArrayUnbind();
  IndexBufferUnbind();
  TextureUnbind(); // Doesn't matter what texture is bound, it will unbind anything.
}

MakeInternal void ObjectUpdate(Object *object, float dt) {
  Body *body = object->body;
  if (body != nullptr) {
    object->current_object_state = 
      IsBodyMoving(object->body) ? object->current_object_state : ObjectState::IDLE;
    if (body->b2_body->GetType() != b2_staticBody) {
      object->position = BodyGetPositionInPixels(object->body);
    }
  }

  if (object->animations[(int)object->current_object_state] != nullptr) {
    AnimationUpdate(object->animations[(int)object->current_object_state], dt); // Yet still may be we don't have animation at all.
  }
}

MakeInternal void ObjectFree(Object *object) {
  IndexBufferFree(object->index_buffer);
  VertexBufferFree(object->vertex_buffer);
  VertexArrayFree(object->vertex_array);
  TextureFree(object->texture);
  // TODO: Free animations here?
  ObjectFreeAnimations(object);
}

MakeInternal void ObjectAddAnimation(Object *object, ObjectState state, Animation *animation) {
  if (object->animation_filled_indexes_size != OBJECT_MAX_ANIMATIONS
    && (int)state < OBJECT_MAX_ANIMATIONS) {
    object->animations[(int)state] = animation;
    object->animation_filled_indexes[object->animation_filled_indexes_size] = (int)state;
    object->animation_filled_indexes_size++;
  }
}

MakeInternal void ObjectFreeAnimations(Object *object) {
  for (int i = 0; i < object->animations_size; ++i) {
    AnimationFree(object->animations[i]);
  }
  ArenaFree(&object->animation_arena);
  memset(&object->animations, 0, object->animations_size * sizeof(Animation *));
  object->animations_size = 0;
}

MakeInternal void ObjectCreateAddAnimation(Object *object, ObjectState state, const std::string& directory) {
  Animation *animation = GetAnimation(&object->animation_arena);
  AnimationAddAllFramesFromDirectory(animation, &object->animation_arena, directory);

  ObjectAddAnimation(object, state, animation);
}

MakeInternal void ObjectRotateY(Object *object, float angle) {
  object->angle = object->angle + angle > 360.0f ? object->angle + angle - 360.0f : object->angle + angle;
}

MakeInternal glm::mat4 ObjectGetModel(Object *object) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(object->position, 0));
  model = glm::rotate(model, glm::radians(object->angle), glm::vec3(0, 1, 0));
  return model;
}

MakeInternal void ObjectFlipY(Object *object, bool flag) { // TODO: Optimize it if do not need to flip.
  int width = object->texture->width;
  int height = object->texture->height;

  std::vector<float> vertices;
  if (!flag) {
    vertices = {
    -width / 2.0f, -height / 2.0f, 0.0f, 0.0f,
    -width / 2.0f, height / 2.0f, 0.0f, 1.0f,
    width / 2.0f, height / 2.0f, 1.0f, 1.0f,
    width / 2.0f, -height / 2.0f, 1.0f, 0.0f
    };
  }
  else {
    vertices = {
    -width / 2.0f, -height / 2.0f, 1.0f, 0.0f,
    -width / 2.0f, height / 2.0f, 1.0f, 1.0f,
    width / 2.0f, height / 2.0f, 0.0f, 1.0f,
    width / 2.0f, -height / 2.0f, 0.0f, 0.0f
    };
  }

  VertexBufferBindData(object->vertex_buffer, &vertices[0], sizeof(float) * vertices.size());
}

MakeInternal void ObjectTranslate(Object *object, float x, float y) {
  object->position.x += x;
  object->position.y += y;
}