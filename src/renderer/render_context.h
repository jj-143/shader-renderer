#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace renderer {

struct Context {
  float iTime;
  glm::mat4* view;
};

}  // namespace renderer