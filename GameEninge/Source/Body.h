#pragma once

#include <box2d\box2d.h>
#include <glm\glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Arena.h"

struct Body {
  b2Body *b2_body;
  VertexBuffer *vertex_buffer;
  IndexBuffer *index_buffer;
  VertexArray *vertex_array;
};

Body *GetBody(Arena *arena, b2World *world, b2BodyType type, float x, float y, int width, int height, float density, float friction);
glm::vec2 BodyGetPositionInPixels(Body *body);
void BodyMove(Body *body, const b2Vec2 &force);
void BodyBind(Body *body);
void BodySetPosition(Body *body, const b2Vec2 &position);
void BodyUnbind();
bool IsBodyMoving(Body *body);
glm::mat4 BodyGetModel(Body *body);