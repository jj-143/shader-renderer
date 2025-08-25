#include "editor_node_input.h"

#include <imgui/imgui.h>

#include <filesystem>
#include <format>

#include "file_dialog.h"
#include "global.h"
#include "popup.h"
#include "side_panel.h"
#include "ui.h"

namespace {

void InputFile(node::Node& node, node::Input& input);
void InputFloat(node::Node& node, node::Input& input);
void SelectFile(node::Node& node, node::Input& input);

}  // namespace

namespace ui::editor {

void RenderNodeInput(node::Node& node, node::Input& input, bool isUniform) {
  ImGui::PushID(&input);

  // Label
  if (isUniform) {
    if (ImGui::Button(input.name.c_str(), {side_panel::FIELD_START - 16, 0})) {
      popup::Open(popup::EDIT_UNIFORM);
    }

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
}

void InputFloat(node::Node& node, node::Input& input) {
  std::string label = std::format("##InputFloat[{}]", input.name);

  ImGui::SetNextItemWidth(-1);

  if (ImGui::DragScalar(label.c_str(), ImGuiDataType_Float,
                        &input.Value<float>(), 0.1f, nullptr, nullptr, "%f")) {
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

}  // namespace