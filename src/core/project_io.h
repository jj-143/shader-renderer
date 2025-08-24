#pragma once

#include <expected>
#include <fstream>

#include "compositor.h"
#include "project.h"

namespace project {

std::expected<ProjectInfo, LoadError> ReadProjectFile(std::string path);

std::expected<void, std::string> WriteProjectInfo(const ProjectInfo& info,
                                                  const std::string& path);

}  // namespace project
