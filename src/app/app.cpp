#include "app.h"

#include <stb/stb_write.h>

#include <filesystem>
#include <format>
#include <string>

#include "node_registry.h"
#include "ops.h"

namespace {

app::App* instance;

}

namespace app {

App::App(const Config config) : config(config) {
  assert(instance == nullptr && "App already instantiated.");
  instance = this;
};

bool App::Init() {
  stbi_flip_vertically_on_write(1);
  if (!ui.InitUI(config.vW, config.vH, config.title)) {
    return false;
  }

  App::LoadAppConfig();

  shaderManager = ShaderManager::Create();

  node::registry::RegisterAllNodes();

  renderer.Init(config.vW, config.vH);
  renderer.InitContext(*shaderManager);

  contextManager =
      std::make_shared<ContextManager>(*renderer.ctx, *shaderManager, timeline);

  ui.viewportTextureID = renderer.renderTexture;
  ui.SetCamera(renderer.camera);

  return true;
}

void App::Run() {
  ui.Startup();

  // Set default Camera position. forward: +Y, right: +X, up: +Z
  renderer.camera.defaultRotation = {0, 0, 90};
  renderer.camera.Reset();

  ops::LoadSingleShaderOrProjectFile(projectPath);

  while (ui.NewFrame()) {
    if (timeline.IsPlaying()) timeline.Update();

    const bool shouldRender =
        !timeline.rendered || ui.navigationMode == ui::NavigationMode::Walk;

    if (shouldRender) {
      renderer.Render(timeline.iTime);
      timeline.rendered = true;
    }

    contextManager->Validate();

    ui.Render();
  }

  ui.Terminate();
}

project::ProjectInfo App::SerializeProject() {
  project::ProjectInfo result;

  for (auto node : renderer.compositor.GetNodes()) {
    result.nodes.push_back(*node);
  }

  return result;
}

void App::LoadAppConfig() {
  // Init Setting values
  setting.output.width = config.vW;
  setting.output.height = config.vH;

  // Output path, use temp dir
  std::string tempPath = std::filesystem::temp_directory_path().string();
  size_t s = tempPath.copy(setting.output.path, MAX_PATH_SIZE);
  setting.output.path[s] = '\0';

  // Others
  projectPath = config.path;
}

App& GetInstance() {
  assert(instance != nullptr && "App not instantiated.");
  return *instance;
}

}  // namespace app
