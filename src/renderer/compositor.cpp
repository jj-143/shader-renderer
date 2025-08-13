#include "compositor.h"

#include <ranges>

#include "compute_shader_node.h"

namespace renderer {

std::vector<std::shared_ptr<node::Node>> Compositor::GetNodes() {
  return nodes;
}

void Compositor::Init() {
  nodes = {std::make_shared<node::ComputeShaderNode>()};

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

void Compositor::Validate() {
  errorLog.clear();
  isValid = true;

  for (auto node : nodes) {
    isValid &= node->isValid;
    if (!node->isValid) {
      errorLog.append(node->errorLog);
    }
  }
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