#include "app.h"

#include <stb/stb_write.h>

#include <filesystem>
#include <format>
#include <string>

#include "global.h"
#include "node_registry.h"
#include "ops.h"

namespace {

app::App* instance;

}

namespace app {

App::App() {
  assert(instance == nullptr && "App already instantiated.");
  instance = this;
};

bool App::Init(const Args& args) {
  App::InitDefaultsWithArgs(args);

  stbi_flip_vertically_on_write(1);

  if (!ui.InitUI(args.vW, args.vH, args.title)) {
    return false;
  }

  shaderManager = ShaderManager::Create();

  node::registry::RegisterAllNodes();

  renderer.Init(args.vW, args.vH);
  renderer.InitContext(*shaderManager);

  contextManager =
      std::make_shared<ContextManager>(*renderer.ctx, *shaderManager, timeline);

  ui.viewportTextureID = renderer.renderTexture;
  ui.SetCamera(renderer.camera);

  return true;
}

void App::Run() {
  ui.Startup();

  LoadProject();

  while (ui.NewFrame()) {
    if (timeline.IsPlaying()) timeline.Update();

    const bool shouldRender =
        !timeline.rendered || renderer.camera.IsWalkMode();

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

// NOTE: Most of these values will be persist on the project file, and will be
// overwritten while loading the project.
void App::InitDefaultsWithArgs(const Args& args) {
  // Init Setting values
  setting.output.width = args.vW;
  setting.output.height = args.vH;

  // Output path, use temp dir
  std::string tempPath = std::filesystem::temp_directory_path().string();
  size_t s = tempPath.copy(setting.output.path, MAX_PATH_SIZE);
  setting.output.path[s] = '\0';

  // Others
  projectPath = args.path;

  // Set default Camera position. forward: +Y, right: +X, up: +Z
  renderer.camera.defaultRotation = {0, 0, 90};
  renderer.camera.Reset();
}

void App::LoadProject() {
  if (projectPath.empty()) {
    ops::LoadProjectFile(global::DEFAULT_PROJECT, true);
    return;
  }

  ops::LoadSingleShaderOrProjectFile(projectPath);
}

App& GetInstance() {
  assert(instance != nullptr && "App not instantiated.");
  return *instance;
}

}  // namespace app
