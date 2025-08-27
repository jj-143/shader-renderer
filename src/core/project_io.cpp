#include "project_io.h"

#include <format>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "logger.h"
#include "node.h"
#include "project.h"
#include "serialize_glm.h"

using json = nlohmann::json;

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

namespace node {

// NodeInfo
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(NodeInfo, name, label, inputs,
                                                uniforms, initialized, active)

// Input, use custom (de)serializer for std::variant
void from_json(const json& j, Input& input) {
  j.at("type").get_to(input.type);

  input.name = j.value("name", "");

  switch (input.type) {
    case InputType::File:
      input.value = j.at("value").template get<std::string>();
      break;
    case InputType::Float:
      input.value = j.at("value").template get<float>();
      break;
    case InputType::Mat4:
      input.value = j.at("value").template get<glm::mat4>();
      break;
    default:
      break;
  }
}

void to_json(json& j, const Input& input) {
  j = json{{"type", input.type}, {"name", input.name}};

  std::visit(
      overloaded{
          [&](auto arg) { j["value"] = arg; },
      },
      input.value);
}

NLOHMANN_JSON_SERIALIZE_ENUM(InputType, {
                                            {InputType::File, "file"},
                                            {InputType::Float, "float"},
                                            {InputType::Mat4, "mat4"},
                                        })

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
