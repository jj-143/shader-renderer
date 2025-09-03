#include <imgui/imgui.h>

#include "side_panel.h"

namespace ui::side_panel {

void Help() {
  ImGui::SeparatorText("Help");

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {4, 8});

  ImGui::Text("Camera");
  ImGui::BulletText("Toggle [Normal, Walk] (Tab)");
  ImGui::BulletText("WASD + Mouse to navigate in Walk Mode");

  ImGui::Spacing();

  ImGui::Text("Timeline");
  ImGui::BulletText("Play/Pause (Space)");

  ImGui::PopStyleVar(2);
}

}  // namespace ui::side_panel
