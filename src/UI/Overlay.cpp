#include "Overlay.h"

#include "app.h"

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

/// Text logger for shader compile error, fully covers viewport
namespace ErrorLog {

void Render(const ImVec2& pos, ImColor textColor) {
  auto& renderer = App::GetInstance().renderer;
  auto& ui = App::GetInstance().ui;

  if (renderer.IsCompileSuccess()) return;

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

  ImGuiChildFlags childFlags = ImGuiChildFlags_AlwaysUseWindowPadding;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor));

  ImGui::SetNextWindowPos(pos);
  ImGui::SetNextWindowBgAlpha(0.85);
  ImGui::BeginChild("ErrorLog", ImVec2(ui.vW, ui.vH), childFlags, windowFlags);
  {
    // Header
    ImGui::TextWrapped("%s", "[Compile Error]");
    ImGui::Spacing();

    // Error log
    ImGui::TextWrapped("%s", renderer.errorLog.c_str());
  }

  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(1);
  ImGui::EndChild();
}

}  // namespace ErrorLog

}  // namespace Overlay