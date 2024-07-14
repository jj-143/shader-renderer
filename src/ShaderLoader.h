#pragma once
#include <string>

#include "Config.h"

GLuint MakeModule(const std::string& filepath, GLuint moduleType);
GLuint MakeShader(const std::string& vertexFilepath,
                  const std::string& fragmentFilepath);
GLuint MakeComputeShader(const std::string& filepath);