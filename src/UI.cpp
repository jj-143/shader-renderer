#include "UI.h"

bool UI::InitUI(const int width, const int height, const char* title) {
  window = CreateWindow(width, height, title);
  if (window == nullptr) return false;

  glfwGetFramebufferSize(window, &vW, &vH);
  glViewport(0, 0, vW, vH);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetCursorPos(window, (float)vW / 2, (float)vH / 2);

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
  ImGui::StyleColorsDark();
  return true;
}

bool UI::NewFrame() {
  if (glfwWindowShouldClose(window)) return false;
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ImGui::SetMouseCursor(ImGuiMouseCursor_None);
  iTime = glfwGetTime();

  DebugStat::Clear();
  DebugStat::Log(std::format("t: {:.2f}", iTime));
  return true;
}

void UI::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  DebugStat::Render();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
}

GLFWwindow* UI::CreateWindow(const int width, const int height,
                             const char* title) {
  GLFWwindow* window;

  if (!glfwInit()) {
    printf("GLFW couldn't start.\n");
    return nullptr;
  }

  window = glfwCreateWindow(width, height, title, NULL, NULL);
  glfwMakeContextCurrent(window);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    printf("Falied to initialize OpenGL context.\n");
    glfwTerminate();
    return nullptr;
  }

  return window;
}

void UI::Terminate() {
  printf("Program terminated.\n");

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

void UI::UpdateCameraControl() {
  auto& rotation = camera->rotation;
  auto& position = camera->position;

  // Move
  glm::vec3 dPos = {0.0f, 0.0f, 0.0f};
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    dPos.x += 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    dPos.y -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    dPos.x -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    dPos.y += 1.0f;
  }

  glm::vec3 movingForward = camera->forwards;
  movingForward.z = 0;
  (void)glm::normalize(movingForward);

  if (glm::length(dPos) > 0.1f) {
    dPos = glm::normalize(dPos);
    position += 0.02f * dPos.x * movingForward;
    position += 0.02f * dPos.y * camera->right;
  }

  // Rotate
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  glfwSetCursorPos(window, vW / 2, vH / 2);

  glm::vec3 dEulers = {0.0f, 0.0f, 0.0f};
  dEulers.y = -0.05f * static_cast<float>(mouseY - vH / 2);
  dEulers.z = -0.05f * static_cast<float>(mouseX - vW / 2);

  rotation.y = fminf(89.0f, fmaxf(-89.0f, rotation.y + dEulers.y));
  rotation.z += dEulers.z;

  if (rotation.z > 360) {
    rotation.z -= 360;
  } else if (rotation.z < 0) {
    rotation.z += 360;
  }

  camera->Update();

  DebugStat::Log(std::format("Position: ({:.1f}, {:.1f}, {:.1f})", position.x,
                             position.y, position.z));
  DebugStat::Log(std::format("Rotation: ({:.1f}, {:.1f}, {:.1f})", rotation.x,
                             rotation.y, rotation.z));
}