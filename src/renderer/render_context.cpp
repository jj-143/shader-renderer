
#include "render_context.h"

#include "node_input.h"
#include "shader.h"

namespace renderer {

void Context::UseShader(Shader& shader, std::vector<node::Input>& uniforms) {
  glUseProgram(shader.program);

  for (auto& uniform : uniforms) {
    UploadUniform(shader, uniform);
  }

  for (auto& [_, uniform] : this->uniforms) {
    UploadUniform(shader, uniform);
  }
}

}  // namespace renderer