#pragma once
#include <imgui/imgui.h>

#include <string>
#include <vector>

class DebugStat {
 public:
  static DebugStat &GetDebugStat();

  static void Clear();
  static void Log(std::string msg);
  static void Render(const ImVec2 &pos, ImColor textColor = IM_COL32_WHITE);

  DebugStat(DebugStat const &) = delete;
  void operator=(DebugStat const &) = delete;

 private:
  std::vector<std::string> messages = {};
  DebugStat() {};
};