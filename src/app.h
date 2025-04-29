#pragma once

#include <string>

#include "Core/Reloader.h"
#include "Core/Setting.h"
#include "Core/Task.h"
#include "Core/Timeline.h"
#include "Renderer/Renderer.h"
#include "UI/UI.h"

class App {
 public:
  struct Config {
    int vW;
    int vH;
    const char* title;
    std::string shaderPath;
  };

  Config config;
  Setting setting;
  Renderer renderer;
  UI::UI ui;
  Timeline timeline;
  Task::TaskManager taskManager;
  Reloader reloader;

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
  static inline App* instance;

  void LoadAppConfig();
};
