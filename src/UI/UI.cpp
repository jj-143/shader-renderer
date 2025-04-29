#include "UI.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <format>
#include <glm/glm.hpp>
#include <string>

#include "../Core/Ops.h"
#include "../Renderer/Camera.h"
#include "../app.h"
#include "DebugStat.h"

namespace {
void RenderTaskStatus();
}  // namespace

namespace UI {
void WindowSizeCallback(GLFWwindow* window, int width, int height) {
  App::GetInstance().ui.OnWindowResize(width, height);
};

bool UI::InitUI(const int width, const int height, const char* title) {
  vW = width, vH = height;
  ImVec2 nW = CalculateWindowSize(ImVec2(vW, vH));
  wW = nW.x, wH = nW.y;
  window = InitWindow(wW, wH, title);
  if (window == nullptr) return false;

  glfwGetFramebufferSize(window, &wW, &wH);
  glViewport(0, 0, wH, wH);

  glfwSetCursorPos(window, (float)wW / 2, (float)wH / 2);
  glfwSetKeyCallback(window, OnKeyPressed);
  glfwSetWindowSizeCallback(window, WindowSizeCallback);

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
  ImGui::StyleColorsDark();

  // Disable ImGui Log / .ini
  // NOTE: These are saved during ImGui::DestroyContext().
  auto& io = ImGui::GetIO();
  io.LogFilename = nullptr;
  io.IniFilename = nullptr;
  return true;
}

void UI::Terminate() {
  printf("Program terminated.\n");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

bool UI::NewFrame() {
  if (glfwWindowShouldClose(window)) return false;
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  const Timeline& timeline = App::GetInstance().timeline;

  DebugStat::Clear();
  DebugStat::Log(std::format("t: {:.1f}", timeline.iTime));

  if (navigationMode == NavigationMode::Walk) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    UpdateCameraControl();
  }
  return true;
}

void UI::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  PushGlobalStyles();

  RenderMainMenu();
  RenderMain();
  RenderStatusBar();

  PopGlobalStyles();

  ImGui::Render();

  glClearColor(GAP_COLOR.x, GAP_COLOR.y, GAP_COLOR.z, GAP_COLOR.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
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

void UI::UpdateStatus(const std::string message) {
  statusMessage = message;
  statusClearTime = glfwGetTime() + STATUS_DURATION;
}

void UI::Quit() { glfwSetWindowShouldClose(window, GLFW_TRUE); }

void UI::OnWindowResize(int width, int height) {
  auto& app = App::GetInstance();

  wW = width, wH = height;
  ImVec2 size = CalculateWindowSize({});
  vW = width - size.x, vH = height - size.y;

  app.renderer.SetSize(vW, vH);

  // Force render to update
  if (app.timeline.IsPaused()) {
    app.timeline.rendered = false;
  }
}

void UI::PopGlobalStyles() {
  // Colors
  ImGui::PopStyleColor(2);
}

void UI::PushGlobalStyles() {
  // Colors
  // BG for the whole window, resulting only coloring the gaps
  ImGui::PushStyleColor(ImGuiCol_WindowBg, GAP_COLOR);
  ImGui::PushStyleColor(ImGuiCol_Text, TEXT_COLOR);
}

void UI::RenderMain() {
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, BG_COLOR);

  {
    RenderViewport();
    RenderSidePanel();
  }

  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor();
}

void UI::RenderMainMenu() {
  const float menuBarSpacing = 8;  // Compensating first MenuBar Item Spacing
  const ImVec2 buttonPadding = {2, 2};
  const ImVec2 containerPos = {INSET.x, INSET.y};
  const ImVec2 containerSize = {wW - INSET.x * 2, MENU_BAR_HEIGHT};
  const ImVec2 innerPos = {INSET.x + MENU_BAR_PADDING.x - menuBarSpacing,
                           INSET.y + MENU_BAR_PADDING.y - buttonPadding.y};
  const ImVec2 innerSize = {wW - innerPos.x, MENU_BAR_HEIGHT};
  const auto containerFlag =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, buttonPadding);
  ImGui::PushStyleColor(ImGuiCol_MenuBarBg, BG_COLOR);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, BG_COLOR);
  ImGui::PushStyleColor(ImGuiCol_Header, MENU_HOVERED);
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, MENU_HOVERED);
  ImGui::PushStyleColor(ImGuiCol_PopupBg, BG_COLOR);

  ImGui::SetNextWindowPos(containerPos);
  ImGui::SetNextWindowSize(containerSize);
  ImGui::Begin("MenuBarContainer", nullptr, containerFlag);
  ImGui::SetNextWindowPos(innerPos);
  ImGui::BeginChild("MenuBar", innerSize, ImGuiChildFlags_None,
                    ImGuiWindowFlags_MenuBar);
  {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open Shader...", "Ctrl O")) {
          Ops::OpenOpenShaderDialog();
        }
        if (ImGui::MenuItem("Reload Shader", "Ctrl R")) {
          Ops::ReloadShader();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Quit", "Ctrl Q")) {
          Ops::Quit();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Show Overlays", "Alt Shift Z", showOverlays)) {
          Ops::ShowOverlays(!showOverlays);
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Render")) {
        if (ImGui::MenuItem("Render Image", "F12")) {
          Ops::Render(false);
        }
        if (ImGui::MenuItem("Render Animation", "Ctrl F12")) {
          Ops::Render(true);
        }
        ImGui::EndMenu();
      }
    }
    ImGui::EndMenuBar();
  }
  ImGui::EndChild();
  ImGui::End();
  ImGui::PopStyleColor(5);
  ImGui::PopStyleVar(3);
}

void UI::RenderStatusBar() {
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, STATUS_BAR_PADDING);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, STATUS_BAR_COLOR);

  ImGui::SetNextWindowPos(ImVec2(INSET.x, wH - INSET.y - STATUS_BAR_HEIGHT));
  ImGui::SetNextWindowSize(ImVec2(wW - 2 * INSET.x, STATUS_BAR_HEIGHT));
  ImGui::Begin("StatusBar", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

  {
    const char* messageContent =
        glfwGetTime() < statusClearTime ? statusMessage.c_str() : "";
    ImGui::Text("%s", messageContent);
  }

  RenderTaskStatus();

  ImGui::End();
  ImGui::PopStyleVar(4);
  ImGui::PopStyleColor();
}

void UI::RenderViewport() {
  ImGui::SetNextWindowPos(
      {INSET.x + MAIN_MARGIN.x, INSET.y + MAIN_MARGIN.y + MENU_BAR_HEIGHT});
  ImGui::SetNextWindowSize(ImVec2(vW, vH));

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

  ImGui::Begin("Viewport", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoResize);

  ImGui::PopStyleVar();

  isViewportFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

  // Viewport texture
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
  ImGui::Image((ImTextureID)viewportTextureID, ImVec2(vW, vH), {0, 1}, {1, 0});
#pragma GCC diagnostic pop

  // Overlays
  if (showOverlays) {
    DebugStat::Render(
        {INSET.x + MAIN_MARGIN.x + LAYOUT_INSET.x,
         INSET.y + MAIN_MARGIN.y + LAYOUT_INSET.y + MENU_BAR_HEIGHT},
        TEXT_COLOR);
  }
  ImGui::End();
}

ImVec2 CalculateWindowSize(ImVec2 viewport) {
  ImVec2 delta = {INSET.x * 2 + MAIN_MARGIN.x * 2, INSET.y * 2};

  // MainMenu
  delta.y += MAIN_MARGIN.y + MENU_BAR_HEIGHT;

  // Side Panel
  delta.x += INNER_GAP + SIDE_PANEL_WIDTH;

  // StatusBar
  delta.y += MAIN_MARGIN.y + STATUS_BAR_HEIGHT;

  return {viewport.x + delta.x, viewport.y + delta.y};
}

GLFWwindow* InitWindow(const int width, const int height, const char* title) {
  GLFWwindow* window;

  if (!glfwInit()) {
    fprintf(stderr, "GLFW couldn't start.\n");
    return nullptr;
  }

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  glfwMakeContextCurrent(window);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    fprintf(stderr, "Falied to initialize OpenGL context.\n");
    glfwTerminate();
    return nullptr;
  }

  return window;
}
}  // namespace UI

namespace {
using namespace UI;
void RenderTaskStatus() {
  const App& app = App::GetInstance();
  const int PROGRESS_WIDTH = 100;
  const int SPACING = 8;
  const int CANCEL_BUTTON_SIZE = 16;

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {SPACING, 0});

  if (app.taskManager.HasTask()) {
    int label_x = app.ui.wW - SIDE_PANEL_PADDING.x - PROGRESS_WIDTH -
                  CANCEL_BUTTON_SIZE - SPACING -
                  ImGui::CalcTextSize(app.taskManager.task->name).x;
    ImGui::SameLine(label_x);
    ImGui::Text("%s", app.taskManager.task->name);
    ImGui::SameLine(app.ui.wW - SIDE_PANEL_PADDING.x - PROGRESS_WIDTH -
                    CANCEL_BUTTON_SIZE);
    ImGui::ProgressBar(app.taskManager.task->progress,
                       ImVec2(PROGRESS_WIDTH, 0));
    ImGui::SameLine();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 2});
    if (ImGui::SmallButton("x")) {
      Ops::CancelTask();
    }
    ImGui::PopStyleVar();
  }
  ImGui::PopStyleVar(2);
}
}  // namespace
