#include "renderer.h"

namespace renderer {

Compositor& Renderer::GetCompositor() { return *compositor; }

void Renderer::Init(int w, int h) { SetSize(w, h); }

void Renderer::InitContext(ShaderManager& shaderManager) {
  ctx = std::make_shared<Context>(compositor, shaderManager, &camera);
}

void Renderer::Render() {
  if (!compositor->isValid) return;

  bool shoudRender = compositor->valueChanged || ctx->forceRender ||
                     !ctx->rendered || ctx->camera->IsWalkMode();
  if (!shoudRender) return;

  ctx->rendered = true;
  ctx->forceRender = false;

  compositor->Execute(*ctx);

  RenderToOutput();
}

void Renderer::SetSize(int width, int height) {
  this->width = width;
  this->height = height;

  compositor->SetSize(width, height);

  if (ctx) {
    ctx->forceRender = true;
  }
}

void Renderer::CopyCompositor(std::shared_ptr<Compositor> target) {
  compositor = target;
  compositor->Init();
  compositor->SetSize(width, height);
  ctx->compositor = compositor;
}

void Renderer::RenderToOutput() {
  /**
   * NOTE:
   * It's effectively acting as an empty "Output Node" or "Copy Shader".
   *
   * Just copy the ID instead of actually copying the buffer or rendering using
   * a pass-through shader.
   */
  renderTexture = compositor->output;
}

}  // namespace renderer