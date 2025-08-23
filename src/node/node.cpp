#include "node.h"

#include "app.h"

namespace node {

void Node::OnShaderFileChanged(const std::string& shaderPath) {
  this->shaderPath = shaderPath;

  auto& app = app::GetInstance();
  app.renderer.ctx->compositor.needValidation = true;
}

}  // namespace node