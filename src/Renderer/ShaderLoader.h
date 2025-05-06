#pragma once
#include <glad/gl.h>

#include <string>

struct ShaderFile {
  std::string path = "";
  bool isUserFile = false;
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