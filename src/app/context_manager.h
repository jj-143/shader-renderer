#pragma once

#include <string>

#include "reloader.h"
#include "render_context.h"
#include "timeline.h"

namespace app {

class ContextManager {
 public:
  renderer::Context& ctx;
  Reloader& reloader;
  Timeline& timeline;
  std::string& projectPath;

  std::string errorLog;

  bool firstValidation = true;

  ContextManager(renderer::Context& ctx, Reloader& reloader, Timeline& timeline,
                 std::string& projectPath)
      : ctx(ctx),
        reloader(reloader),
        timeline(timeline),
        projectPath(projectPath) {}

  void Validate();

 private:
  void SyncValidationResult();

  void ValidateRenderContext();
};

}  // namespace app
