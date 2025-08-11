#include "ops.h"

#include <filesystem>
#include <format>
#include <string>
#include <thread>

#include "app.h"
#include "file_dialog.h"
#include "global.h"
#include "logger.h"
#include "render.h"
#include "task.h"

namespace {

void RenderTask(task::Task& task, bool animation);

}

namespace ops {

bool CancelTask() {
  task::TaskManager& taskManager = App::GetInstance().taskManager;
  if (!taskManager.HasTask()) return false;
  taskManager.CancelTask();
  ops::Report("Canceling [{}]", taskManager.task->name);
  return true;
}

bool LoadShader(std::string path, bool reload) {
  App& app = App::GetInstance();

  if (!fs::exists(path)) {
    ops::ReportError("File Not Exist: {}", path);
    return false;
  }

  // States, before loading this shader file
  bool firstLoad = app.renderer.IsIdle() ||  // Cold start
                   app.shaderPath != path;   // New file
  bool hadErrorLastTime = app.renderer.IsCompileError();

  // Cleanup
  app.timeline.Stop();

  // Initialize
  app.shaderPath = path;
  app.reloader.SetWatchFile(path);

  // Compile shader
  app.renderer.SetComputeShader(app.shaderPath.c_str());

  if (app.renderer.IsCompileError()) {
    ops::ReportError("Compile Error in {}\n\n{}", path, app.renderer.errorLog);
    ops::SetErrorLog(app.renderer.errorLog);
    return false;
  }

  ops::SetErrorLog();

  app.timeline.Play();

  if (firstLoad) {
    ops::Report(path);
    return true;
  }

  if (hadErrorLastTime) {
    ops::Report("Compile Success");
    return true;
  }

  if (reload) {
    ops::Report("Reloaded");
    return true;
  }

  return true;
}

bool OpenOpenShaderDialog() {
  App& app = App::GetInstance();
  fs::path defaultPath = fs::path(app.shaderPath).parent_path();

  std::optional<std::string> outPath = file_dialog::OpenFile(defaultPath);
  if (!outPath) return false;

  LoadSingleShaderProject(outPath.value());
  return true;
}

bool ShowOverlays(bool set) {
  App& app = App::GetInstance();
  app.ui.showOverlays = set;
  return true;
}

bool SetErrorLog(const std::string& content) {
  auto& ui = App::GetInstance().ui;
  ui.errorLog = content;
  return true;
}

bool SetErrorLog() {
  auto& ui = App::GetInstance().ui;
  ui.errorLog.reset();
  return true;
}

bool AlignViewportToOutput() {
  auto& output = App::GetInstance().setting.output;
  ImVec2 newVSize(output.width, output.height);
  ImVec2 newWSize = ui::CalculateWindowSize(newVSize);

  auto& ui = App::GetInstance().ui;
  glfwSetWindowSize(ui.window, newWSize.x, newWSize.y);
  return true;
}

bool AlignOutputToViewport() {
  auto& ui = App::GetInstance().ui;
  ImVec2 wWithoutV = ui::CalculateWindowSize(ImVec2(0, 0));
  ImVec2 newVSize = ImVec2(ui.wW, ui.wH) - wWithoutV;

  auto& output = App::GetInstance().setting.output;
  output.width = newVSize.x;
  output.height = newVSize.y;
  return true;
}

bool MaximizeViewport(bool set) {
  auto& ui = App::GetInstance().ui;

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

bool ReloadShader() {
  App& app = App::GetInstance();
  return ops::LoadShader(app.shaderPath.c_str(), true);
}

bool Render(bool animation) {
  App& app = App::GetInstance();
  if (app.taskManager.HasTask()) return false;
  app.timeline.Pause();
  const char* taskName = animation ? "Render Animation" : "Render Image";
  app.taskManager.RunTask(task::CreateTask(taskName, RenderTask, animation));
  return true;
}

bool ResetCamera() {
  Camera& camera = App::GetInstance().renderer.camera;
  camera.Reset();
  return true;
}

bool Quit() {
  App& app = App::GetInstance();
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
  App& app = App::GetInstance();
  const Output& output = app.setting.output;
  fs::path outDir;
  try {
    outDir = PrepareOutDir(output.path);
  } catch (fs::filesystem_error error) {
    ops::ReportError(error.what());
    return;
  }

  render::RenderContext context;
  render::Params params{
      .shaderPath = app.shaderPath,
      .width = output.width,
      .height = output.height,
  };

  context.Setup(params, app.renderer.camera);
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

    context.Render(iTime);
    if (!context.WriteToFile(currentFilepath.string().c_str())) writeErrors++;
    task.progress = (i + 1.f) / (nFrames);
  }

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
  context.Teardown();
}

}  // namespace
