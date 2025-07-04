#pragma once

#include <string>

#include "Reloader.h"
#include "Renderer.h"
#include "Setting.h"
#include "Task.h"
#include "Timeline.h"
#include "UI.h"

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
  UI::UI ui;
  Timeline timeline;
  Task::TaskManager taskManager;
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
