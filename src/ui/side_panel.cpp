#include "side_panel.h"

#include "ui.h"

namespace {

using namespace ui;

auto WINDOW_FLAGS = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize;

const auto TAB_BAR_PADDING = ImVec2{8, 6};
const auto TAB_BAR_GAP = ImVec2{3, 0};
const float TAB_HEIGHT = LINE_HEIGHT + TAB_BAR_PADDING.y * 2;

const auto& TAB_NORMAL = colors::SHADE_6;
const auto& TAB_ACTIVE = colors::SHADE_8;
const auto& TAB_HOVER = colors::SHADE_8;

}  // namespace

namespace ui::side_panel {

void TabItem(const char* label, std::function<void()> Child) {
  if (ImGui::BeginTabItem(label)) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, SIDE_PANEL_PADDING);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8, 4});
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});
    ImGui::PushStyleColor(ImGuiCol_ChildBg, TAB_ACTIVE);

    ImGui::BeginChild(label, ImVec2(-1, -1),
                      ImGuiChildFlags_AlwaysUseWindowPadding,
                      ImGuiWindowFlags_None);

    Child();

    ImGui::EndChild();

    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(1);

    ImGui::EndTabItem();
  }
}

}  // namespace ui::side_panel

namespace ui {

void SidePanel() {
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
  ImGui::PushStyleVar(ImGuiStyleVar_TabBarBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, TAB_BAR_GAP);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, TAB_BAR_PADDING);

  ImGui::PushStyleColor(ImGuiCol_Tab, TAB_NORMAL);
  ImGui::PushStyleColor(ImGuiCol_TabActive, TAB_ACTIVE);
  ImGui::PushStyleColor(ImGuiCol_TabHovered, TAB_HOVER);

  if (ImGui::BeginTabBar("SidePanelTab",
                         ImGuiTabBarFlags_FittingPolicyScroll)) {
    side_panel::TabItem("Settings", side_panel::Settings);

    ImGui::EndTabBar();
  }

  ImGui::PopStyleVar(4);
  ImGui::PopStyleColor(3);
}

void UI::RenderSidePanel() {
  if (isViewportMaximized) return;

  ImVec2 pos{INSET.x + MAIN_MARGIN.x + vW + INNER_GAP,
             INSET.y + MAIN_MARGIN.y + MENU_BAR_HEIGHT - TAB_HEIGHT};
  ImVec2 size{SIDE_PANEL_WIDTH, vH + TAB_HEIGHT};

  ImGui::SetNextWindowPos(pos);
  ImGui::SetNextWindowSize(size);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, ROUNDING);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, BG_COLOR);

  ImGui::Begin("SidePanel", nullptr, WINDOW_FLAGS);

  SidePanel();

  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor();

  ImGui::End();
}

}  // namespace ui
