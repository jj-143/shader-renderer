#pragma once

#include <string>

namespace project {

struct ProjectInfo {
  std::string path;
  std::string singleShaderPath;
};

enum class LoadError {
  FileRead,
};

}  // namespace project