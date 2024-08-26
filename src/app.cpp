#include "app.h"

#include <stb/stb_write.h>

App::App(const Config config) : config(config) {
  assert(instance == nullptr && "App already instantiated.");
  instance = this;
};

int App::Run() {
  stbi_flip_vertically_on_write(1);
  if (!ui.InitUI(config.vW, config.vH, config.title)) {
    return 1;
  }

  renderer.Init(config.vW, config.vH);
  renderer.SetComputeShader(config.shaderPath.c_str());
  ui.viewportTextureID = renderer.colorbuffer;

  // Set default Camera position
  renderer.camera.rotation = {0, 0, 90};  // forward: +Y, right: +X, up: +Z
  renderer.camera.Update();
  ui.SetCamera(renderer.camera);
  timeline.Play();

  while (ui.NewFrame()) {
    if (timeline.IsPlaying()) timeline.Update();

    const bool shouldRender =
        !timeline.rendered || ui.navigationMode == UI::NavigationMode::WALK;

    if (shouldRender) {
      renderer.Render(timeline.iTime);
      timeline.rendered = true;
    }

    ui.Render();
  }

  ui.Terminate();
  return 0;
}
