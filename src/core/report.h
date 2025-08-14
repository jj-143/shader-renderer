#pragma once

#include <string_view>

#include "app.h"
#include "logger.h"

namespace {

enum class ReportLevel { Info, Error };

void ReportEx(const std::string& message, ReportLevel level) {
  auto& app = app::GetInstance();
  std::string firstLine = message.substr(0, message.find_first_of("\n"));

  app.ui.UpdateStatus(firstLine);

  switch (level) {
    case ReportLevel::Info:
      logger::Info("(Report): {}", message);
      break;
    case ReportLevel::Error:
      logger::Error("(Report): {}", message);
      break;
  }
}

}  // namespace

namespace ops {

/// Display a message.
template <class... Args>
bool Report(const std::string_view& fmt, Args&&... args) {
  ReportEx(std::vformat(fmt, std::make_format_args(args...)),
           ReportLevel::Info);
  return true;
}

/// Display an error message.
template <class... Args>
bool ReportError(const std::string_view& fmt, Args&&... args) {
  ReportEx(std::vformat(fmt, std::make_format_args(args...)),
           ReportLevel::Error);
  return true;
}

}  // namespace ops