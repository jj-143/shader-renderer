#pragma once

#include <memory>
#include <string>

#include "context_manager.h"
#include "renderer.h"
#include "setting.h"
#include "shader_manager.h"
#include "task.h"
#include "timeline.h"
#include "ui.h"

namespace project {
struct ProjectInfo;
}

namespace app {

class App {
 public:
  struct Config {
    int vW;
    int vH;
    const char* title;
    std::string path;
  };

  Setting setting;
  renderer::Renderer renderer;
  ui::UI ui;
  Timeline timeline;

  task::TaskManager taskManager;
  std::shared_ptr<ShaderManager> shaderManager;
  std::shared_ptr<ContextManager> contextManager;

  std::string projectPath;

  App(const Config config);
  App(App const&) = delete;
  void operator=(App const&) = delete;

  /// Return true if success
  bool Init();

  /// Start the main loop
  void Run();

  project::ProjectInfo SerializeProject();

 private:
  Config config;

  void LoadAppConfig();
};

App& GetInstance();

}  // namespace app
