#pragma once

#include <format>
#include <print>
#include <source_location>
#include <string_view>

namespace logger {

template <class... Args>
void Info(std::string_view fmt, Args &&...args) {
  std::println("[Info] {}", std::vformat(fmt, std::make_format_args(args...)));
};

template <class... Args>
void Error(std::string_view fmt, Args &&...args) {
  std::println(stderr,
               "\033[31m"
               "[Error] {}"
               "\033[0m",
               std::vformat(fmt, std::make_format_args(args...)));
};

template <class... Args>
struct Debug {
  Debug(
      std::string_view fmt, Args &&...args,
      const std::source_location &location = std::source_location::current()) {
    std::println(
        "\033[36m"
        "[Debug] {} ({}:{})"
        "\033[0m",
        std::vformat(fmt, std::make_format_args(args...)), location.file_name(),
        location.line());
  };
};

template <class... Args>
Debug(std::string_view, Args &&...) -> Debug<Args...>;

}  // namespace logger
