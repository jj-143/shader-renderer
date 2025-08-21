// Anything for global defaults which cannot be changed during runtime
// or set as "default configuration".
#pragma once

#include <filesystem>
#include <string>

#include "platform_utils.h"
#include "shader_loader.h"

namespace fs = std::filesystem;

// clang-format off
namespace global {

inline const int         DEFAULT_VIEWPORT_WIDTH    = 768;
inline const int         DEFAULT_VIEWPORT_HEIGHT   = 768;
inline const char*       DEFAULT_WINDOW_TITLE      = "Shader Renderer";

inline const fs::path    BINARY_ROOT         = platform_utils::GetBinaryRoot();
inline const std::string DEFAULT_SHADER      = (BINARY_ROOT / "shaders/default.comp").string();

// Internal shader chunks
inline const ShaderSource CHUNK_COMMON       = {.path=(BINARY_ROOT / "res/common.comp").string()};
inline const ShaderSource CHUNK_COMMON_POST  = {.path=(BINARY_ROOT / "res/common-post.comp").string()};

}  // namespace global
// clang-format on
