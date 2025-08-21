#pragma once

#include <optional>
#include <string>

#include "error.h"
#include "gl.h"

enum class ShaderType {
  ComputeShader,
  FragShader,
};

struct Shader {
 public:
  std::string id;
  std::string path;
  GLuint program;
  ShaderType type;

  bool isValid;
  bool isDirty;
  float lastModified;
  std::optional<error::Error> error;
};
