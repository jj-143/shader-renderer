#include "context_manager.h"

#include <format>

#include "compositor.h"
#include "logger.h"
#include "ops.h"

namespace app {

void ContextManager::Update() {
  if (timeline.IsPlaying()) {
    timeline.Update();
    ctx.uniforms["iTime"].value = timeline.iTime;
    ctx.rendered = false;
  }

  ctx.uniforms["view"].value = ctx.camera->view;
}

void ContextManager::Validate() {
  errors.reset();

  ValidateShaderManager();

  ValidateRenderContext();

  SyncValidationResult();
}

void ContextManager::ValidateShaderManager() {
  if (!shaderManager.needRefresh) return;

  logger::Debug("[ContextManager] Validate ShaderManager");

  if (!errors) errors.emplace();

  shaderManager.Refresh(*errors);

  if (!shaderManager.HasCompileErrors()) {
    ctx.compositor->needValidation = true;

    if (!firstValidation) {
      ops::Report("Reloaded");
    }
  }

  if (firstValidation) {
    firstValidation = false;
  }
}

void ContextManager::ValidateRenderContext() {
  if (!ctx.compositor->needValidation) return;

  logger::Debug("[ContextManager] Validate RenderContext");

  if (!errors) errors.emplace();

  ctx.compositor->Validate(ctx, *errors);

  ctx.forceRender = true;
}

void ContextManager::SyncValidationResult() {
  if (!errors) return;

  if (errors->size()) {
    auto errorLog = error::MergeErrors(*errors);

    ops::ReportError("Compositor Error\n{}", errorLog);
    ops::SetErrorLog(errorLog);

    SuspendTimeline();
    return;
  }

  ResumeTimeline();
  ops::SetErrorLog();
}

void ContextManager::SuspendTimeline() {
  if (!wasTimelinePlaying.has_value()) {
    wasTimelinePlaying = timeline.IsPlaying();
  }
  timeline.Pause();
}

void ContextManager::ResumeTimeline() {
  if (wasTimelinePlaying.value_or(false)) {
    timeline.Play();
  }
  wasTimelinePlaying.reset();
}

};  // namespace app
