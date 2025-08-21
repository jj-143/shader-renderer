#pragma once

#include <string>
#include <vector>

#include "node.h"

namespace project {

struct ProjectInfo {
  std::string path;
  std::vector<node::NodeInfo> nodes;
};

enum class LoadError {
  FileRead,
  FileParse,
};

}  // namespace project