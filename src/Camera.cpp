#include "Camera.h"

Camera::Camera() {}

void Camera::Update() {
  const float theta = glm::radians(rotation.z);
  const float phi = glm::radians(rotation.y);

  forwards = {glm::cos(theta) * glm::cos(phi), glm::sin(theta) * glm::cos(phi),
              glm::sin(phi)};
  right = glm::normalize(glm::cross(forwards, globalUp));
  up = glm::normalize(glm::cross(right, forwards));
  view = glm::lookAt(position, position + forwards, up);
}
