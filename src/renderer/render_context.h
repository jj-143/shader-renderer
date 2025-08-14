#pragma once

#include <glm/gtc/type_ptr.hpp>

class ShaderManager;

namespace renderer {

class Compositor;

struct Context {
  Compositor& compositor;
  ShaderManager& shaderManager;

  glm::mat4& view;
  float iTime;

  Context(Compositor& c, ShaderManager& s, glm::mat4& v)
      : compositor(c), shaderManager(s), view(v), iTime(0.f) {}
};

}  // namespace renderer