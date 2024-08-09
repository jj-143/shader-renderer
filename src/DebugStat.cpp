#include "DebugStat.h"

DebugStat& DebugStat::GetDebugStat() {
  static DebugStat instance;
  return instance;
};

void DebugStat::Clear() {
  DebugStat& ds = GetDebugStat();
  ds.messages.clear();
};

void DebugStat::Log(std::string msg) {
  DebugStat& ds = GetDebugStat();
  ds.messages.push_back(msg);
};

void DebugStat::Render(ImColor textColor) {
  DebugStat& ds = GetDebugStat();
  auto& io = ImGui::GetIO();

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoBackground;

  ImGui::SetNextWindowPos(ImVec2(8, 8));
  ImGui::SetNextWindowSize(ImVec2(0, 0));
  ImGui::Begin("Debug Stat", nullptr, windowFlags);
  ImGui::TextColored(textColor, "fps: %.1f", io.Framerate);

  for (auto& line : ds.messages) {
    ImGui::TextColored(textColor, "%s", line.c_str());
  }

  ImGui::End();
}
