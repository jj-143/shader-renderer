
#include "compositor.h"
#include "compute_shader_node.h"
#include "project.h"

namespace renderer {

renderer::Compositor BuildCompositor(project::ProjectInfo info) {
  renderer::Compositor compositor;

  for (const auto& nodeInfo : info.nodes) {
    auto node = std::make_shared<node::ComputeShaderNode>();

    node->SetProgramPath(nodeInfo.shaderPath);

    compositor.nodes.emplace_back(node);
  }

  return compositor;
}

}  // namespace renderer