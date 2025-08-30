
#include "compositor.h"
#include "logger.h"
#include "node_registry.h"
#include "project.h"

namespace renderer {

std::shared_ptr<renderer::Compositor> BuildCompositor(
    project::ProjectInfo info) {
  auto compositor = Compositor::Create();

  for (const auto &nodeInfo : info.nodes) {
    if (auto node = node::registry::CreateNode(nodeInfo); node) {
      compositor->nodes.emplace_back(node);
    }
  }

  return compositor;
}

bool AddNode(const std::string &name, Compositor &compositor) {
  auto entry = node::registry::GetNodeEntry(name);

  if (!entry) {
    logger::Error("No such Node registered: {}", name);
    return false;
  }

  auto node = node::registry::CreateNode(name);
  node->initialized = false;

  node->Init();
  node->SetSize(compositor.width, compositor.height);

  compositor.nodes.push_back(node);
  compositor.needValidation = true;

  return true;
}

bool RemoveNode(node::Node &target, Compositor &compositor) {
  target.toRemove = true;
  compositor.needValidation = true;
  return true;
}

}  // namespace renderer