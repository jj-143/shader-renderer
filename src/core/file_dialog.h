#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace file_dialog {

namespace fs = std::filesystem;
std::optional<std::string> OpenFile(fs::path path);

}  // namespace file_dialog
