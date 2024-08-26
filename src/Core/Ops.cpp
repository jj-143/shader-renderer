#include "Ops.h"

#include <filesystem>
#include <format>
#include <string>
#include <thread>

#include "../Renderer/Render.h"
#include "../app.h"
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

bool Report(std::string message) {
  App& app = App::GetInstance();
  app.ui.UpdateStatus(message);
  return true;
}

bool Render(bool animation) {
  App& app = App::GetInstance();
  if (app.taskManager.HasTask()) return false;
  app.timeline.Pause();
  const char* taskName = animation ? "Render Animation" : "Render Image";
  Ops::Report(std::format("[{:s}]", taskName));
  app.taskManager.RunTask(Task::CreateTask(taskName, RenderTask, animation));
  return true;
}
}  // namespace Ops

// Locals
namespace {
namespace fs = std::filesystem;

// Create directories if needed
fs::path PrepareOutDir(const char* dirpath) {
  fs::path out(dirpath);
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
    Ops::Report(error.what());
    return;
  }

  Render::RenderContext context;
  context.Setup(app.config, app.renderer.camera);
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
    if (!context.WriteToFile(currentFilepath.c_str())) writeErrors++;
    task.progress = (i + 1.f) / (nFrames);
  }

  if (writeErrors > 0) {
    Ops::Report(std::format("ERROR: Coudn't write output {:s} for {:d} {:s}",
                            writeErrors > 1 ? "files" : "file", writeErrors,
                            writeErrors > 1 ? "frames" : "frame"));
  } else {
    Ops::Report(
        animation
            ? std::format("Animation Saved in {:s}", outDir.c_str())
            : std::format("Image Saved to {:s}", currentFilepath.c_str()));
  }
  context.Teardown();
}
}  // namespace
