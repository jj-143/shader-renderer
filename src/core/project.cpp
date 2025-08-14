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
  auto& app = app::GetInstance();

  const std::string& path = info.singleShaderPath;

  if (!std::filesystem::exists(path)) {
    ops::ReportError("File Not Exist: {}", path);
    return false;
  }

  app.shaderPath = path;
  app.contextManager.firstValidation = true;
  app.contextManager.reloader.needReload = true;

  app.reloader.SetWatchFile(path);

  app.renderer.SetComputeShader(app.shaderPath.c_str());

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
