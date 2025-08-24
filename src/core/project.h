#pragma once

#include <string>
#include <vector>

#include "node.h"

namespace project {

struct ProjectInfo {
  std::string path;
  std::vector<node::NodeInfo> nodes;

  // Runtime
  bool temporary = false;
};

enum class LoadError {
  FileRead,
  FileParse,
};

}  // namespace project

namespace ops {

/// Load a project file
bool LoadProjectFile(const std::string& path, bool asTemporary = false);

/// Load a single shader file as a new project
bool LoadSingleShaderProject(const std::string& path);

/// Load a file as a single shader project or as a project file format (.json)
bool LoadSingleShaderOrProjectFile(const std::string& path,
                                   bool asTemporary = false);

/// Save App state, including compositor configurations, into a project file
bool SaveProject(const std::string& path);

}  // namespace ops
