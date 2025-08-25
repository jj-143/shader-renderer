#include "renderer.h"

namespace renderer {

void Renderer::Init(int w, int h) {
  renderTexture = &compositor.output;

  SetSize(w, h);
}

void Renderer::InitContext(ShaderManager& shaderManager) {
  ctx = std::make_shared<Context>(compositor, shaderManager, &camera);
}

void Renderer::Render() {
  if (!compositor.isValid) return;

  bool shoudRender =
      ctx->forceRender || !ctx->rendered || ctx->camera->IsWalkMode();
  if (!shoudRender) return;

  ctx->rendered = true;
  ctx->forceRender = false;

  compositor.Execute(*ctx);
}

void Renderer::SetSize(int width, int height) {
  this->width = width;
  this->height = height;
  compositor.SetSize(width, height);

  if (ctx) {
    ctx->forceRender = true;
  }
}

void Renderer::CopyCompositor(const Compositor& target) {
  compositor = target;
  compositor.Init();
  compositor.SetSize(width, height);
}

}  // namespace renderer