#pragma once

#include <glm/glm.hpp>

class Camera {
 public:
  enum class Mode { Normal, Walk };

  glm::vec3 position{};
  glm::vec3 rotation{};

  glm::vec3 defaultPosition{};
  glm::vec3 defaultRotation{};

  glm::vec3 forwards{};
  glm::vec3 right{};
  glm::vec3 up{};
  glm::mat4 view{};

  glm::vec3 globalUp{0, 0, 1};

  Camera();

  inline bool IsWalkMode() { return mode == Mode::Walk; }
  inline bool IsNormalMode() { return mode == Mode::Normal; }

  /// Reset position & rotation to default values
  void Reset();

  void Update();

  Mode mode = Mode::Normal;

 private:
};