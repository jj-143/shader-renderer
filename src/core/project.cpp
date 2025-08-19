#include "project.h"

#include <expected>
#include <filesystem>
#include <format>

#include "compositor.h"
#include "compute_shader_node.h"
#include "ops.h"

namespace project {

std::expected<ProjectInfo, LoadError> MakeSingleShaderProject(
    const std::string& path) {
  if (!std::filesystem::exists(path)) {
    return std::unexpected(LoadError::FileRead);
  }

  auto nodeInfo = node::ComputeShaderNode::Spec;

  nodeInfo.shaderPath = path;

  return ProjectInfo{
      .path = path,
      .nodes = {nodeInfo},
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
    default:
      break;
  }
}

}  // namespace

bool LoadProjectInfo(const ProjectInfo& info) {
  auto& app = app::GetInstance();
  app.shaderPath = info.path;

  app.renderer.CopyCompositor(renderer::BuildCompositor(info));

  // Reset
  app.renderer.compositor.needValidation = true;
  app.contextManager->firstValidation = true;
  app.timeline.Stop();
  app.timeline.Play();

  return true;
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

}  // namespace ops
