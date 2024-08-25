#include "../Timeline.h"
#include "../app.h"
#include "UI.h"

void DevPanel();
void Help();

void RenderSidePanel() {
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, SIDE_PANEL_PADDING);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 8});
  ImGui::PushStyleColor(ImGuiCol_FrameBg, PANEL_BG_COLOR);
  ImGui::BeginChild("SidePanel", ImVec2(SIDE_PANEL_WIDTH, 0),
                    ImGuiChildFlags_FrameStyle);
  {
    DevPanel();
    Help();
  }
  ImGui::EndChild();
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor();
}

void DevPanel() {
  ImGui::Spacing();
  App &app = App::GetInstance();
  Timeline &timeline = app.timeline;

  ImGui::SeparatorText("Dev Panel");

  if (ImGui::Button(timeline.IsPlaying()   ? "PLAYING"
                    : timeline.IsPaused()  ? "PAUSED"
                    : timeline.IsStopped() ? "STOPPED"
                                           : "")) {
    if (timeline.IsPlaying()) {
      timeline.Pause();
    } else {
      timeline.Play();
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("STOP")) {
    timeline.Stop();
  }
}

void Help() {
  ImGui::SeparatorText("Help");

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {4, 4});

  ImGui::Text("Navigation");
  ImGui::BulletText("Toggle [Walk, Normal] (Tab)");
  ImGui::BulletText("WASD + Mouse to navigate in Walk Mode");

  ImGui::Spacing();

  ImGui::Text("Timeline");
  ImGui::BulletText("Play/Pause (Space)");

  ImGui::PopStyleVar(2);
}