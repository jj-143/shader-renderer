#pragma once

#include <string>

#include "Core/Task.h"
#include "Renderer/Renderer.h"
#include "Setting.h"
#include "Timeline.h"
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

  App(const Config config);
  App(App const&) = delete;
  void operator=(App const&) = delete;

  int Run();

  static inline App& GetInstance() {
    assert(instance != nullptr && "App not instantiated.");
    return *instance;
  }

 private:
  static inline App* instance;
};
