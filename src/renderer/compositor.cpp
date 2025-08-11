#include "compositor.h"

#include "compute_shader_node.h"

namespace renderer {

void Compositor::Init() {
  node = std::make_shared<node::ComputeShaderNode>();
  node->Init();
  output = node->output;
}

void Compositor::Execute(Context& ctx) { this->node->Execute(ctx); }

void Compositor::Validate() {
  isValid = node->isValid;
  errorLog = node->errorLog;
}

void Compositor::SetSize(int width, int height) {
  node->SetSize(width, height);
  this->width = width;
  this->height = height;
}

}  // namespace renderer