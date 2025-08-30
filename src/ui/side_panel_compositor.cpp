#include <filesystem>
#include <optional>
#include <string>

#include "app.h"
#include "compositor.h"
#include "editor_node_input.h"
#include "file_dialog.h"
#include "global.h"
#include "node.h"
#include "node_registry.h"
#include "popup.h"
#include "side_panel.h"
#include "ui.h"

namespace {

const float BIG_BUTTON_HEIGHT = ui::LINE_HEIGHT + 8 * 2;
const float CONTROLS_HEIGHT = BIG_BUTTON_HEIGHT + 8 * 2;  // padded around

const ImColor HEADER_BG = ui::colors::SHADE_5;
const ImColor HEADER_LINE = ui::colors::SHADE_9;
const ImVec4& NODE_BG = ui::colors::SHADE_7;

void Region_CompositorRack(renderer::Compositor& compositor);
void Region_Controls(renderer::Compositor& compositor);

void RenderNode(node::ShaderNode* node);

}  // namespace

namespace ui::side_panel {

void CompositorEditor() {
  auto& compositor = app::GetInstance().renderer.GetCompositor();

  Region_CompositorRack(compositor);

  ImGui::Dummy({0, 8});

  Region_Controls(compositor);
}

}  // namespace ui::side_panel

namespace {

using namespace ui;

void Region_CompositorRack(renderer::Compositor& compositor) {
  ImGui::PushStyleColor(ImGuiCol_ChildBg, {1, 1, 1, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 8});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

  ImGui::BeginChild("CompositorRack", {0, -CONTROLS_HEIGHT},
                    ImGuiChildFlags_AutoResizeY |
                        ImGuiChildFlags_AlwaysAutoResize |
                        ImGuiChildFlags_AlwaysUseWindowPadding);

  for (auto node : compositor.GetNodes()) {
    RenderNode(node.get());
  }

  ImGui::PopStyleColor(1);
  ImGui::PopStyleVar(3);
  ImGui::EndChild();
}

void Region_Controls(renderer::Compositor& compositor) {
  ImGui::PushStyleColor(ImGuiCol_Button, NODE_BG);

  if (ImGui::Button("Add Node...", {-1, BIG_BUTTON_HEIGHT})) {
    ImGui::OpenPopup("AddNode");
  }

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});

  if (ImGui::BeginPopup("AddNode")) {
    for (auto& name : node::registry::GetNodeNames()) {
      if (ImGui::Selectable(name.c_str())) {
        renderer::AddNode(name, compositor);
      }
    }
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::EndPopup();
  }

  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
}

void NodeHeaderBG() {
  float w = ImGui::GetContentRegionAvail().x;  // node width - 2 * padding.x
  ImVec2 pos = ImGui::GetCursorScreenPos();

  ImVec2 start = pos - ImVec2{8, 4};                   // before padding
  ImVec2 end = pos + ImVec2{w + 8, LINE_HEIGHT + 4};   // after padding
  ImVec2 midStart = {start.x, (start.y + end.y) / 2};  // middle of the header

  // BG, rounding rect
  ImGui::GetWindowDrawList()->AddRectFilled(start, end, HEADER_BG, 4);

  // BG no rounding rect for bottom half
  ImGui::GetWindowDrawList()->AddRectFilled(midStart, end, HEADER_BG, 0);

  // Separator line
  ImGui::GetWindowDrawList()->AddLine({start.x, end.y}, end, HEADER_LINE, 1);
}

void NodeHeaderActiveToggle(node::ShaderNode& node) {
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, colors::SHADE_3);
  ImGui::PushStyleColor(ImGuiCol_CheckMark, colors::SHADE_7);

  {
    if (ImGui::Checkbox("##active", &node.active)) {
      node.OnActiveChanged();
    };
  }

  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(2);
}

void NodeHeaderOptionButton(node::ShaderNode& node) {
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8);
  ImGui::PushStyleColor(ImGuiCol_Button, colors::SHADE_3);

  {
    if (ImGui::Button("##options", {LINE_HEIGHT, LINE_HEIGHT})) {
      ImGui::OpenPopup("Options");
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});

    if (ImGui::BeginPopup("Options")) {
      if (ImGui::Selectable("Remove")) {
        renderer::RemoveNode(node, app::GetInstance().renderer.GetCompositor());
      }
      ImGui::SetNextItemWidth(-FLT_MIN);
      ImGui::EndPopup();
    }

    ImGui::PopStyleVar(1);
  }

  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(1);
}

void NodeHeader(node::ShaderNode& node) {
  NodeHeaderBG();

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {4, 0});

  {
    // Smaller horizontal padding for header, compared to 8 for body
    ImGui::NewLine();
    ImGui::SameLine(4);

    // Button: Active
    NodeHeaderActiveToggle(node);

    // Text: Title
    ImGui::SameLine(0, 6);
    auto& title = node.label.empty() ? node.name : node.label;
    ImGui::Text("%s", title.c_str());

    // Button: Options
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - LINE_HEIGHT + 12);
    NodeHeaderOptionButton(node);
  }

  ImGui::PopStyleVar(1);
}

void AddUniform(node::Node& node) {
  ImGui::PushID(&node);

  if (ImGui::Button("Add Uniform", {-1, 0})) {
    popup::Open(popup::EDIT_UNIFORM);
  }

  popup::EditUniform(node, nullptr);

  ImGui::PopID();
}

void NodeBody(node::ShaderNode& node) {
  for (auto& input : node.inputs) {
    editor::RenderNodeInput(node, input);
  }

  ImGui::Spacing();

  ImGui::Separator();

  ImGui::Spacing();

  if (node.uniforms.size()) {
    for (auto& uniform : node.uniforms) {
      editor::RenderNodeInput(node, uniform, true);
    }

    ImGui::Spacing();
  }

  AddUniform(node);
}

void RenderNode(node::ShaderNode* node) {
  ImGui::PushID(node);

  ImGui::PushStyleColor(ImGuiCol_ChildBg, NODE_BG);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 4));
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4);

  auto flags =
      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding;

  ImGui::BeginChild(node->name.c_str(), {0, 0}, flags);

  {
    NodeHeader(*node);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 4});

    ImGui::Dummy({0, 8});

    NodeBody(*node);

    ImGui::Spacing();

    ImGui::PopStyleVar();
  }

  ImGui::PopStyleColor(1);
  ImGui::PopStyleVar(2);
  ImGui::EndChild();
  ImGui::PopID();
}

}  // namespace