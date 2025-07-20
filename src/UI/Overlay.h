#pragma once

#include <imgui/imgui.h>

#include <string>

namespace overlay::stat {

void Clear();
void Log(std::string msg);
void Render(const ImVec2& pos, ImColor textColor = IM_COL32_WHITE);

}  // namespace overlay::stat

namespace overlay::error_log {

void Render(const ImVec2& pos, ImColor textColor);

}  // namespace overlay::error_log
