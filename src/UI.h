#pragma once
#include "Config.h"

class Camera;

class UI {
 public:
  int vW, vH;  // viewport
  float iTime = 0;
  GLFWwindow* window;
  glm::vec3 textColor = glm::vec3(1, 1, 1);

  UI() {};

  bool InitUI(const int width, const int height, const char* title);
  bool NewFrame();
  void Render();
  void SetCamera(Camera& camera) { this->camera = &camera; }
  void UpdateCameraControl();

  static GLFWwindow* CreateWindow(const int width, const int height,
                                  const char* title);
  static void Terminate();

 private:
  Camera* camera;
};