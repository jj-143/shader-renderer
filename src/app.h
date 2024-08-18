#pragma once

#include "Setting.h"
#include "UI.h"

class App {
 public:
  struct Config {
    int vW;
    int vH;
    const char* title;
  };

  Config config;
  Setting setting;
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
