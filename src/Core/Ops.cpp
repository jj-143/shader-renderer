#include "Ops.h"

#include <filesystem>
#include <format>
#include <string>
#include <thread>

#include "../Global.h"
#include "../Renderer/Render.h"
#include "../app.h"
#include "./FileDialog.h"
#include "Task.h"

namespace {
void RenderTask(Task::Task& task, bool animation);
}

namespace Ops {
bool CancelTask() {
  Task::TaskManager& taskManager = App::GetInstance().taskManager;
  if (!taskManager.HasTask()) return false;
  taskManager.CancelTask();
  Ops::Report(std::format("Canceling [{:s}]", taskManager.task->name));
  return true;
}

bool LoadShader(std::string path) {
  App& app = App::GetInstance();

  if (!fs::exists(path)) {
    Ops::ReportError(std::format("File Not Exist: {:s}", path));
    return false;
  }

  app.config.shaderPath = path;

  app.timeline.Stop();
  app.renderer.DeleteShader();

  ShaderCompileResult result =
      app.renderer.SetComputeShader(app.config.shaderPath.c_str());

  if (!result.isSuccess) {
    Ops::ReportError(std::format("Compile Error: {:s}", result.error));
    return false;
  }

  app.timeline.Play();

  app.reloader.SetWatchFile(path);
  return true;
}

bool OpenOpenShaderDialog() {
  App& app = App::GetInstance();
  fs::path defaultPath = fs::path(app.config.shaderPath).parent_path();

  std::optional<std::string> outPath = FileDialog::OpenFile(defaultPath);
  if (!outPath) return false;

  LoadShader(outPath.value());
  return true;
}

bool ShowOverlays(bool set) {
  App& app = App::GetInstance();
  app.ui.showOverlays = set;
  return true;
}

bool ReloadShader() {
  App& app = App::GetInstance();
  return Ops::LoadShader(app.config.shaderPath.c_str());
}

bool Report(std::string message, ReportLevel level) {
  App& app = App::GetInstance();

  std::string firstLine = message.substr(0, message.find_first_of("\n"));

  app.ui.UpdateStatus(firstLine);

  if (level == ReportLevel::Error) {
    fprintf(stderr, "[Error] %s\n", message.c_str());
  } else {
    fprintf(stdout, "[Info] %s\n", message.c_str());
  }
  return true;
}

bool Render(bool animation) {
  App& app = App::GetInstance();
  if (app.taskManager.HasTask()) return false;
  app.timeline.Pause();
  const char* taskName = animation ? "Render Animation" : "Render Image";
  app.taskManager.RunTask(Task::CreateTask(taskName, RenderTask, animation));
  return true;
}

bool Quit() {
  App& app = App::GetInstance();
  app.ui.Quit();
  return true;
}
}  // namespace Ops

// Locals
namespace {
namespace fs = std::filesystem;

/// Create directories if needed.
/// Accepts absolute path without modification;
/// translate relative path as relative from BINARY_ROOT.
fs::path PrepareOutDir(const char* dirpath) {
  fs::path out(dirpath);

  if (out.is_relative()) {
    out = Global::BINARY_ROOT / out;
  }

  if (!fs::is_directory(out)) {
    fs::create_directories(out);
  }
  return out;
}

// iFrame is calculated from current [Timeline.iTime]
// and starts with #0001. e.g, iTime(0) = iFrame(1).
void RenderTask(Task::Task& task, bool animation) {
  App& app = App::GetInstance();
  const Output& output = app.setting.output;
  fs::path outDir;
  try {
    outDir = PrepareOutDir(output.path);
  } catch (fs::filesystem_error error) {
    Ops::ReportError(error.what());
    return;
  }

  Render::RenderContext context;
  Render::Params params{
      .shaderPath = app.config.shaderPath,
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
    Ops::ReportError(
        std::format("ERROR: Coudn't write output {:s} for {:d} {:s}",
                    writeErrors > 1 ? "files" : "file", writeErrors,
                    writeErrors > 1 ? "frames" : "frame"));
  } else {
    Ops::Report(animation ? std::format("Animation Saved in {:s}",
                                        outDir.string().c_str())
                          : std::format("Image Saved to {:s}",
                                        currentFilepath.string().c_str()));
  }
  context.Teardown();
}
}  // namespace
