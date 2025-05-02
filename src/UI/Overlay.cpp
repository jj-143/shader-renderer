#include "Overlay.h"

#include "../app.h"

namespace Overlay {

namespace Stat {

namespace {
std::vector<std::string> messages = {};
}

void Clear() { messages.clear(); };

void Log(std::string msg) { messages.push_back(msg); };

void Render(const ImVec2& pos, ImColor textColor) {
  auto& io = ImGui::GetIO();

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoBackground;

  ImGui::SetNextWindowPos(pos);
  ImGui::BeginChild("Debug Stat", {0, 0},
                    ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX,
                    windowFlags);
  {
    ImGui::TextColored(textColor, "fps: %.1f", io.Framerate);
    for (auto& line : messages) {
      ImGui::TextColored(textColor, "%s", line.c_str());
    }
  }
  ImGui::EndChild();
}

}  // namespace Stat

}  // namespace Overlay