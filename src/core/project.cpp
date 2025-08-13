#include "project.h"

#include <expected>
#include <filesystem>
#include <format>

#include "ops.h"

namespace project {

std::expected<ProjectInfo, LoadError> MakeSingleShaderProject(
    const std::string& path) {
  if (!std::filesystem::exists(path)) {
    return std::unexpected(LoadError::FileRead);
  }
  return ProjectInfo{.singleShaderPath = path};
}

}  // namespace project

namespace ops {

using namespace project;

namespace {

void HandleLoadError(const std::string& path, const LoadError& error) {
  switch (error) {
    case LoadError::FileRead:
      ops::Report("Cannot Read File: {}", path);
      break;
    default:
      break;
  }
}

}  // namespace

bool LoadProjectInfo(const ProjectInfo& info) {
  App& app = App::GetInstance();

  const std::string& path = info.singleShaderPath;
  bool reload = app.shaderPath == info.singleShaderPath;

  if (!std::filesystem::exists(path)) {
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

bool LoadSingleShaderProject(const std::string& path) {
  auto info = MakeSingleShaderProject(path);

  if (!info) {
    HandleLoadError(path, info.error());
    return false;
  }

  return LoadProjectInfo(*info);
}

}  // namespace ops
