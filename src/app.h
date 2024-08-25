#pragma once

#include "Renderer/Renderer.h"
#include "Setting.h"
#include "UI/UI.h"

class App {
 public:
  struct Config {
    int vW;
    int vH;
    const char* title;
  };

  Config config;
  Setting setting;
  Renderer renderer;
  UI ui;

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
