#pragma once

#include <string>
#include <unordered_map>
#include <glm\glm.hpp>

#include "Arena.h"

// TODO: Add uniform location hash

struct Program {
  unsigned int id = 0;
};

Program *GetProgram(Arena *arena);
void ProgramAddShader(Program *program, unsigned int type, const std::string& source);
void ProgramLink(Program *program);
void ProgramBind(Program *program);
void ProgramUnbind();
void ProgramSetUniform4f(Program *program, const std::string &name, float f1, float f2, float f3,
  float f4);
void ProgramSetUniformMat4(Program *program, const std::string &name, const glm::mat4 &mat4);
void ProgramSetUniform1i(Program *program, const std::string &name, int i);