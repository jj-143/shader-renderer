#include "shader.h"

#include "node_input.h"

void UpdateLocation(Shader& shader, const node::Input& uniform) {
  shader.locations[uniform.name] =
      glGetUniformLocation(shader.program, uniform.name.c_str());
}

void UploadUniform(Shader& shader, node::Input& uniform) {
  auto location = shader.locations[uniform.name];

  switch (uniform.type) {
    case node::InputType::Float:
      glUniform1f(location, uniform.Value<float>());
      break;
    case node::InputType::File:
      break;
  }
}
