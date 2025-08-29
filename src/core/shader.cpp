#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    case node::InputType::Mat4:
      glUniformMatrix4fv(location, 1, GL_FALSE,
                         glm::value_ptr(uniform.Value<glm::mat4>()));
      break;
    case node::InputType::Color4:
      glUniform4fv(location, 1, (float*)&(uniform.Value<glm::vec4>()));
      break;
  }
}
