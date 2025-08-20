#include "compositor.h"

#include <ranges>

#include "compute_shader_node.h"

namespace renderer {

std::vector<std::shared_ptr<node::ShaderNode>> Compositor::GetNodes() {
  return nodes;
}

Compositor Compositor::Clone() {
  Compositor cloned = *this;

  cloned.nodes.clear();

  for (auto node : nodes) {
    cloned.nodes.emplace_back(node->Clone());
  }

  return cloned;
}

void Compositor::Init() {
  for (auto node : nodes) {
    node->Init();
  }

  LinkNodes();
}

void Compositor::Execute(Context& ctx) {
  for (auto node : nodes) {
    node->Execute(ctx);
  }
}

void Compositor::Validate(Context& ctx, std::vector<error::Error>& errors) {
  isValid = true;

  for (auto node : nodes) {
    node->Validate(ctx);

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

void Compositor::LinkNodes() {
  if (!nodes.size()) return;

  auto lastOutput = nodes[0]->output;

  for (auto node : nodes | std::views::drop(1)) {
    node->input = lastOutput;
    lastOutput = node->output;
  }

  output = lastOutput;
}

}  // namespace renderer