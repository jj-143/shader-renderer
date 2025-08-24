#include "project.h"

#include <expected>
#include <filesystem>
#include <format>

#include "compositor.h"
#include "node_registry.h"
#include "ops.h"
#include "project_io.h"

namespace project {

std::expected<ProjectInfo, LoadError> MakeSingleShaderProject(
    const std::string& path) {
  if (!std::filesystem::exists(path)) {
    return std::unexpected(LoadError::FileRead);
  }

  auto entry = node::registry::GetNodeEntry("ComputeShaderNode");
  if (!entry) return std::unexpected(LoadError::FileRead);

  auto nodeInfo = entry->spec;

  nodeInfo.shaderPath = path;

  return ProjectInfo{
      .path = path,
      .nodes = {nodeInfo},
      .temporary = true,
  };
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
    case LoadError::FileParse:
      ops::Report("Cannot Parse File: {}", path);
      break;
    default:
      break;
  }
}

}  // namespace

bool LoadProjectInfo(const ProjectInfo& info) {
  auto& app = app::GetInstance();
  app.projectPath = info.path;
  app.isTemporaryProject = info.temporary;

  app.renderer.CopyCompositor(renderer::BuildCompositor(info));

  // Reset
  app.renderer.compositor.needValidation = true;
  app.contextManager->firstValidation = true;
  app.timeline.Stop();
  app.timeline.Play();

  return true;
}

bool LoadProjectFile(const std::string& path, bool asTemporary) {
  auto info = ReadProjectFile(path);

  if (!info) {
    HandleLoadError(path, info.error());
    return false;
  }

  info->path = path;
  info->temporary = asTemporary;

  ops::Report("Load: {}", path);

  return LoadProjectInfo(*info);
}

bool LoadSingleShaderProject(const std::string& path) {
  auto info = MakeSingleShaderProject(path);

  if (!info) {
    HandleLoadError(path, info.error());
    return false;
  }

  ops::Report("Load: {}", path);

  return LoadProjectInfo(*info);
}

bool LoadSingleShaderOrProjectFile(const std::string& path, bool asTemporary) {
  std::filesystem::path p(path);

  if (p.extension() == ".json") {
    return ops::LoadProjectFile(p, asTemporary);
  } else {
    return ops::LoadSingleShaderProject(p);
  }
}

bool SaveProject(const std::string& path) {
  auto& app = app::GetInstance();

  if (auto result = WriteProjectInfo(app.SerializeProject(), path); !result) {
    ops::ReportError(" {}", result.error());
    return false;
  }

  app.isTemporaryProject = false;
  app.projectPath = path;

  ops::Report("Saved: {}", path);
  return true;
}

}  // namespace ops
