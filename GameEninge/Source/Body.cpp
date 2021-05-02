#include "Body.h"
#include "Utils.h"
#include "Config.h"

#include <glm\gtx\transform.hpp>
#include <iostream>

MakeInternal b2Body *GetB2Body(b2World *world, b2BodyType type, float x, float y, int width, int height, float density, float friction) {
  b2Body *body = nullptr;

  b2BodyDef body_def;
  body_def.type = type;
  body_def.angle = 0;
  body_def.position = { x * Global_P2M, y * Global_P2M };

  body = world->CreateBody(&body_def);

  b2PolygonShape body_shape;
  body_shape.SetAsBox(width * 0.5f * Global_P2M, height * 0.5f * Global_P2M);

  b2FixtureDef fixture_def;
  fixture_def.shape = &body_shape;
  fixture_def.friction = friction;
  fixture_def.density = type == b2_dynamicBody ? density : 0.0f;

  body->CreateFixture(&fixture_def);

  return body;
}

MakeInternal VertexBuffer *BodyGetDefaultVertexBuffer(Arena *arena, b2Body *body) {
  VertexBuffer *vertex_buffer = nullptr;

  b2Vec2 vertices[4];
  for (int i = 0; i < 4; ++i) {
    vertices[i] = ((b2PolygonShape *)body->GetFixtureList()->GetShape())->m_vertices[i];
    vertices[i] *= Global_M2P;
  }

  vertex_buffer = GetVertexBuffer(arena, &vertices[0], 4 * sizeof(b2Vec2));

  return vertex_buffer;
}

MakeInternal IndexBuffer *BodyGetDefaultIndexBuffer(Arena *arena) {
  IndexBuffer *index_buffer = nullptr;

  unsigned int indices[] = {
    0, 1, 2, 3, 0
  };

  index_buffer = GetIndexBuffer(arena, &indices[0], sizeof(indices) / sizeof(unsigned int));

  return index_buffer;
}

MakeInternal VertexArray *BodyGetDefaultVertexArray(Arena *arena, VertexBuffer *vertex_buffer) {
  VertexArray *vertex_array = nullptr;

  VertexBufferLayout vbl;
  vbl.Push<float>(2);

  vertex_array = GetVertexArray(arena);
  VertexArrayAddBuffer(vertex_array, vertex_buffer, &vbl);

  return vertex_array;
}

MakeInternal Body *GetBody(Arena *arena, b2World *world, b2BodyType type, float x, float y, int width, int height, float density, float friction) {
  Body *body = (Body *)ArenaAlloc(arena, sizeof(Body));

  body->b2_body = GetB2Body(world, type, x, y, width, height, density, friction);
  body->vertex_buffer = BodyGetDefaultVertexBuffer(arena, body->b2_body);
  body->vertex_array = BodyGetDefaultVertexArray(arena, body->vertex_buffer);
  body->index_buffer = BodyGetDefaultIndexBuffer(arena);

  return body;
}

MakeInternal glm::vec2 BodyGetPositionInPixels(Body *body) {
  b2Vec2 position = body->b2_body->GetPosition();
  position *= Global_M2P;
  return { position.x, position.y };
}

MakeInternal void BodyMove(Body *body, const b2Vec2 &force) {
  body->b2_body->ApplyForceToCenter(force, true);
}

MakeInternal void BodyBind(Body *body) {
  VertexArrayBind(body->vertex_array);
  IndexBufferBind(body->index_buffer);
}

MakeInternal void BodyUnbind() {
  VertexArrayUnbind();
  IndexBufferUnbind();
}

MakeInternal bool IsBodyMoving(Body *body) {
  return body->b2_body->GetLinearVelocity().Length() > Global_MovementEpsilon;
}

MakeInternal glm::mat4 BodyGetModel(Body *body) {
  glm::mat4 model = glm::mat4(1.0f);
  glm::vec2 position = BodyGetPositionInPixels(body);
  model = glm::translate(model, glm::vec3(position, 0));
  return model;
}

MakeInternal void BodySetPosition(Body *body, const b2Vec2 &position) {
  body->b2_body->SetTransform(b2Vec2(position.x * Global_P2M, position.y * Global_P2M), 0);
}