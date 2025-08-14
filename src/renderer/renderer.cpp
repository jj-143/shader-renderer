#include "renderer.h"

namespace renderer {

void Renderer::Init(int w, int h) {
  compositor.Init();

  renderTexture = &compositor.output;

  SetSize(w, h);
}

void Renderer::InitContext(ShaderManager& shaderManager) {
  ctx = std::make_shared<Context>(compositor, shaderManager, camera.view);
}

void Renderer::Render(float iTime) {
  if (!compositor.isValid) return;
  ctx->iTime = iTime;
  compositor.Execute(*ctx);
}

void Renderer::SetSize(int width, int height) {
  compositor.SetSize(width, height);
}

void Renderer::SetComputeShader(const char* path) {
  auto nodes = compositor.GetNodes();

  if (nodes.size()) {
    // DEV: single hard-coded node for now
    nodes[0]->SetProgramPath(path);
  }
}

}  // namespace renderer