#include "editor_node_input.h"

#include <imgui/imgui.h>

#include <filesystem>
#include <format>
#include <glm/glm.hpp>

#include "file_dialog.h"
#include "global.h"
#include "popup.h"
#include "side_panel.h"
#include "ui.h"

namespace {

const auto COLOR_EDIT_FLAGS = ImGuiColorEditFlags_AlphaBar |
                              ImGuiColorEditFlags_AlphaPreviewHalf |
                              ImGuiColorEditFlags_AlphaPreview;

void InputFile(node::Node& node, node::Input& input);
void InputFloat(node::Node& node, node::Input& input);
void InputColor4(node::Node& node, node::Input& input);
void SelectFile(node::Node& node, node::Input& input);
void UniformLabelTooltip(node::Input& uniform);

}  // namespace

namespace ui::editor {

void RenderNodeInput(node::Node& node, node::Input& input, bool isUniform) {
  ImGui::PushID(&input);

  // Label
  if (isUniform) {
    if (ImGui::Button(input.name.c_str(), {side_panel::FIELD_START - 16, 0})) {
      popup::Open(popup::EDIT_UNIFORM);
    }

    UniformLabelTooltip(input);

    popup::EditUniform(node, &input);

  } else {
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", input.name.c_str());
  }

  // Field
  ImGui::SameLine(side_panel::FIELD_START);

  switch (input.type) {
    case node::InputType::File:
      InputFile(node, input);
      break;
    case node::InputType::Float:
      InputFloat(node, input);
      break;
    case node::InputType::Color4:
      InputColor4(node, input);
      break;
    default:
      ImGui::NewLine();
      break;
  }

  ImGui::PopID();
}

}  // namespace ui::editor

namespace {

using namespace ui;

void InputFile(node::Node& node, node::Input& input) {
  std::filesystem::path path{input.Value<std::string>()};
  std::string text = path.empty() ? "(empty)" : path.filename().string();

  if (ImGui::Button(text.c_str(), {-1, 0})) {
    SelectFile(node, input);
  };

  ImGui::SetItemTooltip("%s", path.string().c_str());
}

void InputFloat(node::Node& node, node::Input& input) {
  std::string label = std::format("##InputFloat[{}]", input.name);
  float& value = input.Value<float>();

  ImGui::SetNextItemWidth(-1);

  if (ImGui::DragScalar(label.c_str(), ImGuiDataType_Float, &value,
                        ui::SLIDER_SPEED_FLOAT, nullptr, nullptr, "%f")) {
    node.OnUniformChanged();
  }

  ImGui::SetItemTooltip("(float) %f", value);
}

void InputColor4(node::Node& node, node::Input& input) {
  auto label = std::format("##InputColor4[{}]", input.name);
  auto color = (float*)&input.Value<glm::vec4>();

  ImGui::SetNextItemWidth(-1);
  if (ImGui::ColorEdit4(label.c_str(), color, COLOR_EDIT_FLAGS)) {
    node.OnUniformChanged();
  }
}

void SelectFile(node::Node& node, node::Input& input) {
  std::filesystem::path dialogPath{input.Value<std::string>()};

  if (dialogPath.empty()) {
    dialogPath = (global::BINARY_ROOT / "shaders");
  } else {
    dialogPath = std::filesystem::absolute(dialogPath.parent_path());
  }

  auto selected = file_dialog::OpenFile(dialogPath.string());
  if (!selected) return;

  node.OnInputChange(input, selected.value());
}

void UniformLabelTooltip(node::Input& uniform) {
  switch (uniform.type) {
    case node::InputType::Float:
      ImGui::SetItemTooltip("(float) %f", uniform.Value<float>());
      break;
    case node::InputType::Color4: {
      auto v = uniform.Value<glm::vec4>();
      ImGui::SetItemTooltip("(vec4) (%f, %f, %f, %f)", v.x, v.y, v.z, v.w);
      break;
    }
    default:
      break;
  }
}

}  // namespace