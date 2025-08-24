#include "project_io.h"

#include <format>
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

std::expected<void, std::string> WriteProjectInfo(const ProjectInfo& info,
                                                  const std::string& path) {
  std::ofstream ofs;
  ofs.open(path);

  if (!ofs) return std::unexpected("Cannot open file for save");

  std::string content;

  try {
    json root = info;
    content = root.dump(2);
  } catch (json::exception e) {
    return std::unexpected("JSON dump failed");
  }

  ofs << content;

  if (!ofs) return std::unexpected("Cannot write to file");

  return {};
}

}  // namespace project
