
#include "compositor.h"
#include "compute_shader_node.h"
#include "project.h"

namespace renderer {

renderer::Compositor BuildCompositor(project::ProjectInfo info) {
  renderer::Compositor compositor;

  auto node = std::make_shared<node::ComputeShaderNode>();

  node->SetProgramPath(info.singleShaderPath);

  compositor.nodes = {node};

  return compositor;
}

}  // namespace renderer