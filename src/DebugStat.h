#pragma once
#include "Config.h"

class DebugStat {
 public:
  static DebugStat &GetDebugStat();

  static void Clear();
  static void Log(std::string msg);
  static void Render();

  DebugStat(DebugStat const &) = delete;
  void operator=(DebugStat const &) = delete;

 private:
  std::vector<std::string> messages = {};
  DebugStat() {};
};