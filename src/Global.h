// Anything for Global defaults which cannot be changed during runtime
// or set as "default configuration".
#pragma once

#include <filesystem>
#include <string>

#include "PlatformUtils.h"

namespace fs = std::filesystem;

// clang-format off
namespace Global {
inline const int         DEFAULT_VIEWPORT_WIDTH    = 768;
inline const int         DEFAULT_VIEWPORT_HEIGHT   = 768;
inline const char*       DEFAULT_WINDOW_TITLE      = "Shader Renderer";

inline const fs::path    BINARY_ROOT         = PlatformUtils::GetBinaryRoot();
inline const std::string DEFAULT_SHADER      = BINARY_ROOT / "res/shaders/default.comp";
}  // namespace Global
// clang-format on
