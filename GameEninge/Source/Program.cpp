#include <GL\glew.h>
#include <iostream>
#include <fstream>

#include "Program.h"
#include "Utils.h"

MakeInternal std::string ReadShader(const std::string &source) {
  std::ifstream input(source);

  if (!input) {
    ASSERT("Couldn't open shader file!");
  }

  std::string shader_data(
    (std::istreambuf_iterator<char>(input)),
    std::istreambuf_iterator<char>());

  return shader_data;
}

MakeInternal unsigned int CompileShader(unsigned int type, const std::string &source) {
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glCall(glShaderSource(id, 1, &src, nullptr));
  glCall(glCompileShader(id));

  int result;
  glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (!result) {
    int length;
    glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char *message = (char *)alloca(length * sizeof(char));
    glCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile "
      << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
      << "shader!\n";
    std::cout << message << '\n';
    glCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

MakeInternal int GetUniformLocation(Program *program, const std::string& name) {
  glCall(int location = glGetUniformLocation(program->id, name.c_str()));
  ASSERT(location != -1);
  return location;
}

MakeInternal Program *GetProgram(Arena *arena) {
  Program *program = (Program *)ArenaAlloc(arena, sizeof(Program));
  
  program->id = glCall(glCreateProgram());

  return program;
}

MakeInternal void ProgramAddShader(Program *program, unsigned int type, const std::string& source) {
  ASSERT(source.size() != NULL);

  std::string shader_source = ReadShader(source);
  unsigned int shader_id = CompileShader(type, shader_source);

  ASSERT(shader_id != 0);
  glCall(glAttachShader(program->id, shader_id));
}

MakeInternal void ProgramLink(Program *program) {
  glCall(glLinkProgram(program->id));
  glCall(glValidateProgram(program->id));
}

MakeInternal void ProgramBind(Program *program) {
  glCall(glUseProgram(program->id));
}

MakeInternal void ProgramUnbind() {
  glCall(glUseProgram(0));
}

MakeInternal void ProgramSetUniform4f(Program *program, const std::string &name, float f1, float f2, float f3,
  float f4) {
  glCall(glUniform4f(GetUniformLocation(program, name), f1, f2, f3, f4));
}

MakeInternal void ProgramSetUniformMat4(Program *program, const std::string &name, const glm::mat4 &mat4) {
  glCall(glUniformMatrix4fv(GetUniformLocation(program, name), 1, GL_FALSE, &mat4[0][0]));
}

MakeInternal void ProgramSetUniform1i(Program *program, const std::string &name, int i) {
  glCall(glUniform1i(GetUniformLocation(program, name), i));
}