#include "app.h"

#include <stb/stb_write.h>

#include "Core/Ops.h"

App::App(const Config config) : config(config) {
  assert(instance == nullptr && "App already instantiated.");
  instance = this;
};

bool App::Init() {
  stbi_flip_vertically_on_write(1);
  if (!ui.InitUI(config.vW, config.vH, config.title)) {
    return false;
  }

  renderer.Init(config.vW, config.vH);
  ui.viewportTextureID = renderer.colorbuffer;
  ui.SetCamera(renderer.camera);

  reloader.Start();
  reloader.WatchForChange(config.shaderPath);
  return true;
}

void App::Run() {
  // Set default Camera position
  renderer.camera.rotation = {0, 0, 90};  // forward: +Y, right: +X, up: +Z
  renderer.camera.Update();

  renderer.SetComputeShader(config.shaderPath.c_str());
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
    reloader.HandleReload();
  }

  ui.Terminate();
}
