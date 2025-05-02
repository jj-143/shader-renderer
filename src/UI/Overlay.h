#pragma once
#include <imgui/imgui.h>

#include <string>

namespace Overlay {

namespace Stat {

void Clear();
void Log(std::string msg);
void Render(const ImVec2 &pos, ImColor textColor = IM_COL32_WHITE);

}  // namespace Stat

}  // namespace Overlay