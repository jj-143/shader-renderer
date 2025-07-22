#pragma once

#include <string>

#include "reloader.h"
#include "renderer.h"
#include "setting.h"
#include "task.h"
#include "timeline.h"
#include "ui.h"

class App {
 public:
  struct Config {
    int vW;
    int vH;
    const char* title;
    std::string shaderPath;
  };

  Setting setting;
  Renderer renderer;
  ui::UI ui;
  Timeline timeline;
  task::TaskManager taskManager;
  Reloader reloader;

  std::string shaderPath;

  App(const Config config);
  App(App const&) = delete;
  void operator=(App const&) = delete;

  /// Return true if success
  bool Init();

  /// Start the main loop
  void Run();

  static inline App& GetInstance() {
    assert(instance != nullptr && "App not instantiated.");
    return *instance;
  }

 private:
  Config config;
  static inline App* instance;

  void LoadAppConfig();
};
