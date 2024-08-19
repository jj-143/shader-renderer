#pragma once
#include <glm/glm.hpp>

class Camera {
 public:
  glm::vec3 position{};
  glm::vec3 rotation{};

  glm::vec3 forwards{};
  glm::vec3 right{};
  glm::vec3 up{};
  glm::mat4 view{};

  glm::vec3 globalUp{0, 0, 1};

  Camera();
  void Update();
};