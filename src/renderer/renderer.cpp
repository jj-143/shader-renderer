#include "renderer.h"

namespace renderer {

void Renderer::Init(int w, int h) {
  compositor.Init();

  renderTexture = &compositor.output;
  ctx.view = &camera.view;

  SetSize(w, h);

  initialized = true;
  state = State::RUNNING;
}

void Renderer::Render(float iTime) {
  if (!compositor.isValid) return;
  ctx.iTime = iTime;
  compositor.Execute(ctx);
}

void Renderer::SetSize(int width, int height) {
  compositor.SetSize(width, height);
}

void Renderer::SetComputeShader(const char* path) {
  compositor.node->SetProgramPath(path);
  Validate();
}

void Renderer::Validate() {
  compositor.Validate();
  errorLog = compositor.errorLog;
  state = compositor.isValid ? State::RUNNING : State::COMPILE_ERROR;
}

}  // namespace renderer