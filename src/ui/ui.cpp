#include "ui.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <format>
#include <glm/glm.hpp>
#include <string>

#include "app.h"
#include "camera.h"
#include "logger.h"
#include "ops.h"
#include "overlay.h"

namespace {

void RenderTaskStatus();

}  // namespace

namespace ui {

void WindowSizeCallback([[maybe_unused]] GLFWwindow* window, int width,
                        int height) {
  app::GetInstance().ui.OnWindowResize(width, height);
};

bool UI::InitUI(const int width, const int height, const char* title) {
  vW = width, vH = height;
  ImVec2 nW = CalculateWindowSize(ImVec2(vW, vH));
  wW = nW.x, wH = nW.y;
  window = InitWindow(wW, wH, title);
  if (window == nullptr) return false;

  glfwGetFramebufferSize(window, &wW, &wH);
  glViewport(0, 0, wH, wH);

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
  logger::Info("Terminate program..");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

bool UI::NewFrame() {
  if (glfwWindowShouldClose(window)) return false;
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  const Timeline& timeline = app::GetInstance().timeline;

  overlay::stat::Clear();
  overlay::stat::Log(std::format("t: {:.1f}", timeline.iTime));

  if (navigationMode == NavigationMode::Walk) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    UpdateCameraControl();
  }
  return true;
}

void UI::Startup() {
  // Trigger first frame for focusing window to be working
  NewFrame();
  Render();

  glfwSetCursorPos(window, (float)wW / 2, (float)wH / 2);
  ImGui::SetWindowFocus("Viewport");

  // VSync
  glfwSwapInterval(1);
}

void UI::Render() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

  HandleFocusViewport();
}

void UI::MaximizeViewport(bool set) {
  isViewportMaximized = set;
  // Trigger layout update
  OnWindowResize(wW, wH);
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

  overlay::stat::Log(std::format("Position: ({:.1f}, {:.1f}, {:.1f})",
                                 position.x, position.y, position.z));
  overlay::stat::Log(std::format("Rotation: ({:.1f}, {:.1f}, {:.1f})",
                                 rotation.x, rotation.y, rotation.z));
}

void UI::UpdateStatus(const std::string message) {
  statusMessage = message;
  statusClearTime = glfwGetTime() + STATUS_DURATION;
}

void UI::Quit() { glfwSetWindowShouldClose(window, GLFW_TRUE); }

void UI::OnWindowResize(int width, int height) {
  auto& app = app::GetInstance();

  wW = width, wH = height;

  if (isViewportMaximized) {
    vW = wW;
    vH = wH;
  } else {
    ImVec2 size = CalculateWindowSize({});
    vW = width - size.x;
    vH = height - size.y;
  }

  app.renderer.SetSize(vW, vH);

  // Force render to update
  if (app.timeline.IsPaused()) {
    app.timeline.rendered = false;
  }
}

void UI::HandleFocusViewport() {
  if (!shouldFocusViewport) return;
  shouldFocusViewport = false;
  ImGui::SetWindowFocus("Viewport");
}

void UI::PopGlobalStyles() {
  ImGui::PopStyleColor(14);
  ImGui::PopStyleVar(1);
}

void UI::PushGlobalStyles() {
  // Colors
  // BG for the whole window, resulting only coloring the gaps
  ImGui::PushStyleColor(ImGuiCol_WindowBg, GAP_COLOR);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, GAP_COLOR);

  ImGui::PushStyleColor(ImGuiCol_Header, MENU_SELECTED);
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, MENU_HOVERED);
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ITEM_ACTIVE);
  ImGui::PushStyleColor(ImGuiCol_PopupBg, BG_COLOR);

  ImGui::PushStyleColor(ImGuiCol_Button, ITEM_NORMAL);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ITEM_HOVERED);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ITEM_ACTIVE);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, ITEM_NORMAL);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ITEM_ACTIVE);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ITEM_HOVERED);

  ImGui::PushStyleColor(ImGuiCol_Text, TEXT_COLOR);
  ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ITEM_HOVERED);

  // Styles
  // Mostly for input widgets
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, FRAME_PADDING);
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
  if (isViewportMaximized) return;

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
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, buttonPadding);
  ImGui::PushStyleColor(ImGuiCol_MenuBarBg, BG_COLOR);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, BG_COLOR);

  ImGui::SetNextWindowPos(containerPos);
  ImGui::SetNextWindowSize(containerSize);
  ImGui::Begin("MenuBarContainer", nullptr, containerFlag);
  ImGui::SetNextWindowPos(innerPos);
  ImGui::BeginChild("MenuBar", innerSize, ImGuiChildFlags_None,
                    ImGuiWindowFlags_MenuBar);
  {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open Project / Shader File...", "Ctrl O")) {
          ops::OpenOpenProjectDialog();
        }

        if (ImGui::MenuItem("Reload Project", "Ctrl R")) {
          ops::ReloadProject();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Save Project", "Ctrl S")) {
          ops::SaveCurrentProject();
        }

        if (ImGui::MenuItem("Save Project As...", "Ctrl Shift S")) {
          ops::OpenSaveAsDialog();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Quit", "Ctrl Q")) {
          ops::Quit();
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Toggle Maximize Viewport", "Ctrl Space",
                            isViewportMaximized)) {
          ops::MaximizeViewport(!isViewportMaximized);
        }

        if (ImGui::MenuItem("Show Overlays", "Alt Shift Z", showOverlays)) {
          ops::ShowOverlays(!showOverlays);
        }

        // Viewport
        ImGui::Separator();

        if (ImGui::MenuItem("Align View to Render Output", "Home")) {
          ops::AlignViewportToOutput();
        }

        if (ImGui::MenuItem("Align Output to Viewport", "Ctrl Alt Home")) {
          ops::AlignOutputToViewport();
        }

        // Camera
        ImGui::Separator();

        if (ImGui::MenuItem("Reset Camera", "Alt G")) {
          ops::ResetCamera();
        }

        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Render")) {
        if (ImGui::MenuItem("Render Image", "F12")) {
          ops::Render(false);
        }
        if (ImGui::MenuItem("Render Animation", "Ctrl F12")) {
          ops::Render(true);
        }
        ImGui::EndMenu();
      }
    }
    ImGui::EndMenuBar();
  }
  ImGui::EndChild();
  ImGui::End();
  ImGui::PopStyleColor(2);
  ImGui::PopStyleVar(3);
}

void UI::RenderStatusBar() {
  if (isViewportMaximized) return;

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
  ImVec2 pos = {INSET.x + MAIN_MARGIN.x,
                INSET.y + MAIN_MARGIN.y + MENU_BAR_HEIGHT};
  ImVec2 size(vW, vH);

  if (isViewportMaximized) {
    pos = {0, 0};
    size = ImVec2(wW, wH);
  }

  ImGui::SetNextWindowPos(pos);
  ImGui::SetNextWindowSize(size);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

  ImGui::Begin("Viewport", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoResize);

  ImGui::PopStyleVar();

  isViewportFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

  // Viewport texture
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
  ImGui::Image((ImTextureID)*viewportTextureID, size, {0, 1}, {1, 0});
#pragma GCC diagnostic pop

  // Overlays
  if (showOverlays) {
    // Stat
    {
      overlay::stat::Render(pos + LAYOUT_INSET, TEXT_COLOR);
    }

    // Render output region
    {
      const Output& output = app::GetInstance().setting.output;

      // show only if it's not fit (different ratio)
      if (output.height * vW != vH * output.width) {
        // Note: viewport width = showing 100% of Render output width
        // = cropped or letterboxed vertically.
        ImVec2 size(vW, float(output.height) / output.width * vW);

        // Center position
        ImVec2 iPos = pos + ImVec2(0, (vH - size.y) / 2);

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        static const ImColor color(TEXT_COLOR);

        draw_list->AddRect(iPos, iPos + size, color, 0.0f, ImDrawFlags_None,
                           1.0);
      }
    }
  }

  // Screen - Covers Viewport and always visible if any (unaffected by
  // `showOverlays`)
  {
    // Same pos & size as Viewport
    overlay::error_log::Render(pos, TEXT_COLOR);
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

GLFWwindow* InitWindow(const int width, const int height, const char* title,
                       GLFWwindow* share) {
  GLFWwindow* window;

  if (!glfwInit()) {
    logger::Error("GLFW couldn't start.");
    return nullptr;
  }

  window = glfwCreateWindow(width, height, title, nullptr, share);
  glfwMakeContextCurrent(window);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    logger::Error("Falied to initialize OpenGL context.");
    glfwTerminate();
    return nullptr;
  }

  return window;
}

}  // namespace ui

namespace {

using namespace ui;

void RenderTaskStatus() {
  const auto& app = app::GetInstance();
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
      ops::CancelTask();
    }
    ImGui::PopStyleVar();
  }
  ImGui::PopStyleVar(2);
}

}  // namespace
