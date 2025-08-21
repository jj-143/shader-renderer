#pragma once

#include <glad/gl.h>

#include <string>

struct ShaderSource {
  std::string path = "";
  std::string text = "";
};

struct ShaderCompileResult {
  bool isSuccess;
  GLuint program;
  std::string error = "";
};

ShaderCompileResult MakeModule(const std::string& filepath, GLuint moduleType);
GLuint MakeShader(const std::string& vertexFilepath,
                  const std::string& fragmentFilepath);
ShaderCompileResult MakeComputeShader(const std::string& filepath);

void DeleteProgram(GLuint program);