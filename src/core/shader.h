#pragma once

#include <concepts>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "error.h"
#include "gl.h"

namespace node {
class Input;
}

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
  std::unordered_map<std::string, GLuint> locations;
};

void UploadUniform(Shader& shader, node::Input& uniform);

void UpdateLocation(Shader& shader, const node::Input& uniform);

template <std::ranges::forward_range R>
  requires std::same_as<std::ranges::range_value_t<R>, node::Input>
void UpdateLocations(Shader& shader, const R& uniforms) {
  for (const node::Input& uniform : uniforms) {
    UpdateLocation(shader, uniform);
  }
}
