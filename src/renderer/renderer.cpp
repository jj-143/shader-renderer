#include "renderer.h"

namespace renderer {

void Renderer::Init(int w, int h) {
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
  this->width = width;
  this->height = height;
  compositor.SetSize(width, height);
}

void Renderer::CopyCompositor(const Compositor& target) {
  compositor = target;
  compositor.Init();
  compositor.SetSize(width, height);
}

}  // namespace renderer