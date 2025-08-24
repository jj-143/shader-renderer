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

void Node::OnActiveChanged() {
  auto& app = app::GetInstance();

  if (this->active && !this->isValid) {
    // NOTE: Node initialized with inactive state may have never been
    // validated, e.g, Node's state restored as not active
    app.renderer.ctx->compositor.needValidation = true;
  }
}

}  // namespace node