#include "node.h"

#include "app.h"

namespace node {

void Node::OnShaderFileChanged(const std::string& shaderPath) {
  this->shaderPath = shaderPath;

  // NOTE: If there's other inputs that allows "lazy initialized", such as file
  // input, it should also check the rest
  initialized = true;

  auto& app = app::GetInstance();
  app.renderer.ctx->compositor.needValidation = true;
}

}  // namespace node