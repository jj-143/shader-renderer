#include "project_io.h"

#include <nlohmann/json.hpp>

#include "logger.h"
#include "node.h"
#include "project.h"

using json = nlohmann::json;

namespace node {

// NodeInfo
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(NodeInfo, name, label,
                                                shaderPath, initialized, active)

}  // namespace node

namespace project {

// ProjectInfo
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ProjectInfo, nodes)

std::expected<ProjectInfo, LoadError> ReadProjectFile(std::string path) {
  if (!std::filesystem::exists(path)) {
    return std::unexpected(LoadError::FileRead);
  }

  std::ifstream ifs(path);

  try {
    json data = json::parse(ifs);
    return data.template get<ProjectInfo>();
  } catch (json::exception e) {
    logger::Error("JSON parse error: {}", e.what());
    return std::unexpected(LoadError::FileParse);
  }
}

void SaveProjectInfo(const ProjectInfo& info, const std::string& path) {
  std::ofstream ofs;
  ofs.open(path);

  json root = info;

  ofs << root.dump(2) << std::endl;
  ofs.close();
}

}  // namespace project
