#pragma once

#include <array>

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
  char            path[128]           = "/tmp/";
  Format          format              = FormatPNG;
};
// clang-format on

class Setting {
 public:
  Output output;
};
