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

struct Args {
  int vW;
  int vH;
  const char* title;
  std::string path;
};

class App {
 public:
  Setting setting;
  renderer::Renderer renderer;
  ui::UI ui;
  Timeline timeline;

  task::TaskManager taskManager;
  std::shared_ptr<ShaderManager> shaderManager;
  std::shared_ptr<ContextManager> contextManager;

  bool isTemporaryProject = false;
  std::string projectPath;

  App();
  App(App const&) = delete;
  void operator=(App const&) = delete;

  /// Return true if success
  bool Init(const Args& args);

  /// Start the main loop
  void Run();

  project::ProjectInfo SerializeProject();

 private:
  void InitDefaultsWithArgs(const Args& args);

  void LoadProject();
};

App& GetInstance();

}  // namespace app
