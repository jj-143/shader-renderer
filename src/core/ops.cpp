#include "ops.h"

#include <filesystem>
#include <format>
#include <string>
#include <thread>

#include "app.h"
#include "file_dialog.h"
#include "file_renderer.h"
#include "global.h"
#include "logger.h"
#include "task.h"

namespace {

void RenderTask(task::Task& task, bool animation);

}

namespace ops {

bool DeferUITask(std::function<void()> task) {
  auto& app = app::GetInstance();
  app.ui.QueueDeferredTask(task);
  return true;
}

bool CancelTask() {
  task::TaskManager& taskManager = app::GetInstance().taskManager;
  if (!taskManager.HasTask()) return false;
  taskManager.CancelTask();
  ops::Report("Canceling [{}]", taskManager.task->name);
  return true;
}

bool OpenOpenProjectDialog() {
  auto& app = app::GetInstance();
  fs::path defaultPath = fs::path(app.projectPath).parent_path();

  std::optional<std::string> outPath = file_dialog::OpenFile(defaultPath);
  if (!outPath) return false;

  LoadSingleShaderOrProjectFile(outPath.value());
  return true;
}

bool OpenSaveAsDialog() {
  auto& app = app::GetInstance();
  auto defaultPath = fs::path(app.projectPath).parent_path();

  auto savePath = file_dialog::SaveFile(defaultPath);
  if (!savePath) return false;

  return SaveProject(savePath.value());
}

bool SaveCurrentProject() {
  auto& app = app::GetInstance();

  bool newSave = app.isTemporaryProject || app.projectPath.empty() ||
                 // Protect the default project file
                 // in case of manually loaded
                 app.projectPath == global::DEFAULT_PROJECT;

  if (newSave) return ops::OpenSaveAsDialog();

  // Overwrite
  return SaveProject(app.projectPath);
}

bool ShowOverlays(bool set) {
  auto& app = app::GetInstance();
  app.ui.showOverlays = set;
  return true;
}

bool SetErrorLog(const std::string& content) {
  auto& ui = app::GetInstance().ui;
  ui.errorLog = content;
  return true;
}

bool SetErrorLog() {
  auto& ui = app::GetInstance().ui;
  ui.errorLog.reset();
  return true;
}

bool AlignViewportToOutput() {
  auto& output = app::GetInstance().setting.output;
  ImVec2 newVSize(output.width, output.height);
  ImVec2 newWSize = ui::CalculateWindowSize(newVSize);

  auto& ui = app::GetInstance().ui;
  glfwSetWindowSize(ui.window, newWSize.x, newWSize.y);
  return true;
}

bool AlignOutputToViewport() {
  auto& ui = app::GetInstance().ui;
  ImVec2 wWithoutV = ui::CalculateWindowSize(ImVec2(0, 0));
  ImVec2 newVSize = ImVec2(ui.wW, ui.wH) - wWithoutV;

  auto& output = app::GetInstance().setting.output;
  output.width = newVSize.x;
  output.height = newVSize.y;
  return true;
}

bool MaximizeViewport(bool set) {
  auto& ui = app::GetInstance().ui;

  ui.MaximizeViewport(set);

  // Auto hide overlays upon entering
  static bool oldShowOverlays = ui.showOverlays;

  if (set) {
    oldShowOverlays = ui.showOverlays;
    ui.showOverlays = false;
  } else {
    ui.showOverlays = oldShowOverlays;
  }

  // Focus Viewport
  ui.shouldFocusViewport = true;

  return true;
}

bool ReloadProject() {
  auto& app = app::GetInstance();
  return LoadSingleShaderOrProjectFile(app.projectPath.c_str(),
                                       app.isTemporaryProject);
}

bool Render(bool animation) {
  auto& app = app::GetInstance();
  if (app.taskManager.HasTask()) return false;

  app.timeline.Pause();
  app.ui.InitRenderWindow();

  const char* taskName = animation ? "Render Animation" : "Render Image";
  app.taskManager.RunTask(task::CreateTask(taskName, RenderTask, animation));
  return true;
}

bool ResetCamera() {
  Camera& camera = app::GetInstance().renderer.camera;
  camera.Reset();
  return true;
}

bool Quit() {
  auto& app = app::GetInstance();
  app.ui.Quit();
  return true;
}

}  // namespace ops

// Locals
namespace {

namespace fs = std::filesystem;

/// Create directories if needed.
/// Accepts absolute path without modification;
/// translate relative path as relative from BINARY_ROOT.
fs::path PrepareOutDir(const char* dirpath) {
  fs::path out(dirpath);

  if (out.is_relative()) {
    out = global::BINARY_ROOT / out;
  }

  if (!fs::is_directory(out)) {
    fs::create_directories(out);
  }
  return out;
}

// iFrame is calculated from current [Timeline.iTime]
// and starts with #0001. e.g, iTime(0) = iFrame(1).
void RenderTask(task::Task& task, bool animation) {
  auto& app = app::GetInstance();
  const Output& output = app.setting.output;
  fs::path outDir;
  try {
    outDir = PrepareOutDir(output.path);
  } catch (fs::filesystem_error error) {
    ops::ReportError(error.what());
    return;
  }

  if (!app.ui.renderWindow) {
    ops::ReportError("Render failed: No Render Window");
    return;
  }

  output::FileRenderer fileRenderer;
  output::FileRendererParams params{
      .shaderManager = *app.shaderManager,
      .compositor = app.renderer.GetCompositor(),
      .uniforms = app.renderer.ctx->uniforms,
      .width = output.width,
      .height = output.height,
  };

  auto errors =
      fileRenderer.Setup(params, app.renderer.camera, *app.ui.renderWindow);

  if (errors) {
    auto errorLog = error::MergeErrors(*errors);
    ops::ReportError("Render failed\n{}", errorLog);
    ops::SetErrorLog(errorLog);
    return;
  }

  fs::path currentFilepath;
  int writeErrors = 0;

  const int iFrameNow = app.timeline.iTime * output.frameRate + 1;
  const int frameStart = animation ? output.frameStart : iFrameNow;
  const int frameEnd = animation ? output.frameEnd : iFrameNow;
  const int nFrames = frameEnd + 1 - frameStart;
  const float iTimeDelta = 1 / output.frameRate;

  for (int i = 0; i < nFrames; i++) {
    if (task.isCanceled) return;
    const int iFrame = frameStart + i;
    const float iTime = iFrame * iTimeDelta;
    const auto filename = std::format("{:04d}{:s}", iFrame, output.format.ext);
    currentFilepath = outDir / filename;

    fileRenderer.Render(iTime);
    if (!fileRenderer.WriteToFile(currentFilepath.string().c_str()))
      writeErrors++;
    task.progress = (i + 1.f) / (nFrames);
  }

  app.ui.DestroyRenderWindow();

  if (writeErrors > 0) {
    ops::ReportError("ERROR: Coudn't write output {} for {} {}",
                     writeErrors > 1 ? "files" : "file", writeErrors,
                     writeErrors > 1 ? "frames" : "frame");
  } else {
    if (animation) {
      ops::Report("Animation Saved in {}", outDir.string());
    } else {
      ops::Report("Image Saved to {}", currentFilepath.string());
    }
  }
}

}  // namespace
