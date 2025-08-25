#include "node.h"

#include "app.h"

namespace node {

Input* NodeInfo::GetInput(const std::string& name) {
  if (auto result = std::ranges::find_if(
          inputs, [&name](Input& item) { return item.name == name; });
      result != inputs.end()) {
    return &*result;
  }

  return nullptr;
}

void Node::OnInputChange(Input& input, const InputValue& value) {
  input.value = value;

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