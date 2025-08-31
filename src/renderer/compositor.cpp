#include "compositor.h"

#include <ranges>

#include "shader.h"

namespace renderer {

std::shared_ptr<Compositor> Compositor::Create() {
  return std::shared_ptr<Compositor>(new Compositor());
}

std::vector<std::shared_ptr<node::ShaderNode>> Compositor::GetNodes() {
  return nodes;
}

std::shared_ptr<Compositor> Compositor::GetShared() {
  return shared_from_this();
}

std::shared_ptr<Compositor> Compositor::Clone() {
  auto cloned = std::make_shared<Compositor>(*this);

  cloned->nodes.clear();

  for (auto node : this->nodes) {
    auto clonedNode = node->Clone();
    clonedNode->compositor = cloned;
    cloned->nodes.emplace_back(clonedNode);
  }

  return cloned;
}

void Compositor::Init() {
  for (auto node : nodes) {
    node->Init();
  }
}

void Compositor::Execute(Context& ctx) {
  GLuint lastOutput = 0;

  for (auto node : nodes) {
    if (!node->initialized || !node->active) continue;

    node->input = lastOutput;

    node->Execute(ctx);

    lastOutput = node->output;
  }

  output = lastOutput;
}

void Compositor::Validate(Context& ctx, std::vector<error::Error>& errors) {
  isValid = true;

  std::erase_if(
      nodes, [](std::shared_ptr<node::Node> node) { return node->toRemove; });

  for (auto node : nodes) {
    if (!node->initialized || !node->active) continue;

    node->Validate(ctx);
    node->UpdateUniformLocations();

    for (auto shader : node->shaders) {
      UpdateLocations(*shader, std::views::values(ctx.uniforms));
    }

    if (!node->isValid) {
      isValid = false;

      // Generic invalid node error
      errors.emplace_back(error::Error{
          .label = "Node Error",
          .log = "Invalid",
      });
    }
  }

  needValidation = false;
}

void Compositor::SetSize(int width, int height) {
  this->width = width;
  this->height = height;

  for (auto node : nodes) {
    node->SetSize(width, height);
  }
}

}  // namespace renderer