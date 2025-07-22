#pragma once

#include <array>

constexpr size_t MAX_PATH_SIZE = 256;

struct Format {
  const char* label;
  const char* ext;
};

static Format FormatPNG = {"PNG", ".png"};
static std::array<Format, 1> SUPPORTED_FORMATS = {FormatPNG};

// clang-format off
struct Output {
  int             frameStart          = 1;
  int             frameEnd            = 30;
  float           frameRate           = 30;
  char            path[MAX_PATH_SIZE] = "/tmp/";
  Format          format              = FormatPNG;
  int             width               = 512;
  int             height              = 512;
};
// clang-format on

class Setting {
 public:
  Output output;
};
