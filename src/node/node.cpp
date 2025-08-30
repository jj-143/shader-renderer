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

Input* NodeInfo::GetUniform(const std::string& name) {
  if (auto result = std::ranges::find_if(
          uniforms, [&name](Input& uniform) { return uniform.name == name; });
      result != uniforms.end()) {
    return &*result;
  }

  return nullptr;
}

void Node::RegisterShaders(std::vector<std::shared_ptr<Shader>> shaders) {
  this->shaders = shaders;
}

void Node::UpdateUniformLocations() {
  for (auto shader : shaders) {
    if (!shader) continue;
    UpdateLocations(*shader, uniforms);
  }
}

void Node::UseShader(renderer::Context& ctx, std::shared_ptr<Shader> shader) {
  if (!shader) return;
  ctx.UseShader(*shader, uniforms);
}

void Node::OnInputChange(Input& input, const InputValue& value) {
  input.value = value;

  // NOTE: If there's other inputs that allows "lazy initialized", such as file
  // input, it should also check the rest
  initialized = true;

  auto& app = app::GetInstance();
  app.renderer.ctx->compositor->needValidation = true;
}

void Node::OnActiveChanged() {
  auto& app = app::GetInstance();

  if (this->active && !this->isValid) {
    // NOTE: Node initialized with inactive state may have never been
    // validated, e.g, Node's state restored as not active
    app.renderer.ctx->compositor->needValidation = true;
  }
}

void Node::OnUniformChanged() {
  auto& app = app::GetInstance();
  app.renderer.ctx->forceRender = true;
}

void Node::AddUniform(node::Input uniform) {
  uniforms.emplace_back(uniform);
  UpdateUniformLocations();
}

void Node::EditUniform(node::Input& target, node::Input newUniform) {
  if (target.type == newUniform.type) {
    newUniform.value = target.value;
  }

  if (target.name == newUniform.name) {
    for (auto shader : shaders) {
      shader->locations.erase(target.name);
    }
  }

  target = newUniform;
  UpdateUniformLocations();
}

void Node::RemoveUniform(node::Input& target) {
  std::erase_if(uniforms, [&target](node::Input& uniform) {
    return &uniform == &target;
  });
  UpdateUniformLocations();
}

}  // namespace node