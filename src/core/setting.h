#pragma once

#include <array>
#include <string>

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
  std::string     path                = "";
  Format          format              = FormatPNG;
  int             width               = 512;
  int             height              = 512;
};
// clang-format on

class Setting {
 public:
  Output output;
};
