#include "context_manager.h"

#include <format>

#include "compositor.h"
#include "logger.h"
#include "ops.h"

namespace app {

void ContextManager::Validate() {
  if (!reloader.needReload) return;

  logger::Debug("[ContextManager] Valiate");

  reloader.needReload = false;

  ValidateRenderContext();

  SyncValidationResult();
}

void ContextManager::SyncValidationResult() {
  timeline.Stop();

  errorLog = ctx.compositor->errorLog;

  if (!ctx.compositor->isValid) {
    ops::ReportError("Compile Error\n{}", errorLog);
    ops::SetErrorLog(errorLog);
    return;
  }

  if (firstValidation) {
    firstValidation = false;
    ops::Report("Loaded: {}", projectPath);
  } else {
    ops::Report("Reloaded");
  }

  ops::SetErrorLog();

  timeline.Play();
}

void ContextManager::ValidateRenderContext() { ctx.compositor->Validate(); }

};  // namespace app
