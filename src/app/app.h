#pragma once

#include <string>

#include "context_manager.h"
#include "reloader.h"
#include "renderer.h"
#include "setting.h"
#include "task.h"
#include "timeline.h"
#include "ui.h"

namespace app {

class App {
 public:
  struct Config {
    int vW;
    int vH;
    const char* title;
    std::string shaderPath;
  };

  Setting setting;
  renderer::Renderer renderer;
  ui::UI ui;
  Timeline timeline;
  task::TaskManager taskManager;
  Reloader reloader;
  ContextManager contextManager{renderer.ctx, reloader, timeline, shaderPath};

  std::string shaderPath;

  App(const Config config);
  App(App const&) = delete;
  void operator=(App const&) = delete;

  /// Return true if success
  bool Init();

  /// Start the main loop
  void Run();

 private:
  Config config;

  void LoadAppConfig();
};

App& GetInstance();

}  // namespace app
