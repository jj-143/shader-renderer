#pragma once
#include <glad/gl.h>

#include <string>

GLuint MakeModule(const std::string& filepath, GLuint moduleType);
GLuint MakeShader(const std::string& vertexFilepath,
                  const std::string& fragmentFilepath);
GLuint MakeComputeShader(const std::string& filepath);