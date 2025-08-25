#pragma once

#include "camera.h"

class ShaderManager;

namespace renderer {

class Compositor;

struct Context {
  Compositor& compositor;
  ShaderManager& shaderManager;
  Camera* camera;

  bool rendered = false;
  bool forceRender = false;

  float iTime;

  Context(Compositor& c, ShaderManager& s, Camera* cam)
      : compositor(c), shaderManager(s), camera(cam), iTime(0.f) {}
};

}  // namespace renderer