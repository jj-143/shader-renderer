#pragma once

#include <expected>
#include <fstream>

#include "compositor.h"
#include "project.h"

namespace project {

std::expected<ProjectInfo, LoadError> ReadProjectFile(std::string path);

void SaveProjectInfo(const ProjectInfo& info, const std::string& path);

}  // namespace project
