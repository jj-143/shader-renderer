#pragma once

#include <optional>
#include <string>

#include "error.h"
#include "gl.h"

struct Shader {
  public:
  std::string id;
  std::string path;
  GLuint program;

  bool isValid;
  bool isDirty;
  float lastModified;
  std::optional<error::Error> error;
};
