#include "../Timeline.h"
#include "../app.h"
#include "UI.h"

void DevPanel();

void RenderSidePanel() {
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, SIDE_PANEL_PADDING);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, PANEL_BG_COLOR);
  ImGui::BeginChild("SidePanel", ImVec2(SIDE_PANEL_WIDTH, 0),
                    ImGuiChildFlags_FrameStyle);
  {
    DevPanel();  //
  }
  ImGui::EndChild();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
}

void DevPanel() {
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
