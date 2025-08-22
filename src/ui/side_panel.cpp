#include "side_panel.h"

#include "ui.h"

namespace ui {

void UI::RenderSidePanel() {
  if (isViewportMaximized) return;
  ImGui::SetNextWindowPos({INSET.x + MAIN_MARGIN.x + vW + INNER_GAP,
                           INSET.y + MAIN_MARGIN.y + MENU_BAR_HEIGHT});
  ImGui::SetNextWindowSize(ImVec2(SIDE_PANEL_WIDTH, vH));

  // Window styles
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(SIDE_PANEL_PADDING));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, ROUNDING);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, PANEL_BG_COLOR);

  ImGui::Begin("SidePanel", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoResize);

  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor();
  // END Window styles

  // Panel Contents
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8, 4});
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});

  {
    side_panel::Settings();
  }

  ImGui::PopStyleVar(3);
  // END Panel Contents

  ImGui::End();
}

}  // namespace ui
