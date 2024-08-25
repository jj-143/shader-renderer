#include "UI.h"

void RenderSidePanel() {
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, SIDE_PANEL_PADDING);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, PANEL_BG_COLOR);
  ImGui::BeginChild("SidePanel", ImVec2(SIDE_PANEL_WIDTH, 0),
                    ImGuiChildFlags_FrameStyle);
  {
    ImGui::Text("SidePanel");  //
  }
  ImGui::EndChild();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
}
