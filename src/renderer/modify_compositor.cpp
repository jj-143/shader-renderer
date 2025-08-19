
#include "compositor.h"
#include "node_registry.h"
#include "project.h"

namespace renderer {

renderer::Compositor BuildCompositor(project::ProjectInfo info) {
  renderer::Compositor compositor;

  for (const auto &nodeInfo : info.nodes) {
    if (auto node = node::registry::CreateNode(nodeInfo); node) {
      compositor.nodes.emplace_back(node);
    }
  }

  return compositor;
}

}  // namespace renderer