#pragma once

#include <optional>
#include <string>
#include <vector>

#include "error.h"
#include "render_context.h"
#include "shader_manager.h"
#include "timeline.h"

namespace app {

class ContextManager {
 public:
  renderer::Context& ctx;
  ShaderManager& shaderManager;
  Timeline& timeline;

  std::optional<std::vector<error::Error>> errors;

  bool firstValidation = true;
  bool needEvaluation = true;

  ContextManager(renderer::Context& ctx, ShaderManager& shaderManager,
                 Timeline& timeline)
      : ctx(ctx), shaderManager(shaderManager), timeline(timeline) {}

  void Validate();

 private:
  std::optional<bool> wasTimelinePlaying;

  void ValidateShaderManager();

  void ValidateRenderContext();

  void SyncValidationResult();

  void SuspendTimeline();

  void ResumeTimeline();
};

}  // namespace app
