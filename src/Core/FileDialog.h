#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace FileDialog {
namespace fs = std::filesystem;
std::optional<std::string> OpenFile(fs::path path);

}  // namespace FileDialog
