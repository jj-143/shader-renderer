#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace renderer {

class Compositor;

struct Context {
  Compositor* compositor;

  float iTime;
  glm::mat4* view;
};

}  // namespace renderer