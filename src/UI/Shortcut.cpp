#include <format>

#include "../Core/Ops.h"
#include "../Core/Timeline.h"
#include "../app.h"
#include "UI.h"

namespace UI {
// static function [GLFWkeyfun] for relaying
void UI::OnKeyPressed(GLFWwindow* window, int key, int scancode, int action,
                      int mods) {
  if (action != GLFW_PRESS) return;
  UI& ui = App::GetInstance().ui;
  switch (key) {
    case GLFW_KEY_TAB:
      if (!ui.isViewportFocused) break;
      ui.ToggleNavigationMode();
      break;
    case GLFW_KEY_SPACE:
      if (mods == GLFW_MOD_CONTROL) {
        Ops::MaximizeViewport(!ui.isViewportMaximized);
        break;
      }
      if (ui.isViewportFocused) {
        ui.ToggleTimelinePlay();
        break;
      }
      break;
    case GLFW_KEY_ESCAPE:
      Ops::CancelTask();
      break;
    case GLFW_KEY_F12:
      if (mods == GLFW_MOD_CONTROL) {
        Ops::Render(true);
      } else {
        Ops::Render(false);
      }
      break;
    case GLFW_KEY_G:
      if (mods == GLFW_MOD_ALT) {
        Ops::ResetCamera();
      }
      break;
    case GLFW_KEY_O:
      if (mods == GLFW_MOD_CONTROL) {
        Ops::OpenOpenShaderDialog();
      }
      break;
    case GLFW_KEY_R:
      if (mods == GLFW_MOD_CONTROL) {
        Ops::ReloadShader();
      }
      break;
    case GLFW_KEY_Q:
      if (mods == GLFW_MOD_CONTROL) {
        Ops::Quit();
      }
      break;
    case GLFW_KEY_Z:
      if (mods == (GLFW_MOD_ALT | GLFW_MOD_SHIFT)) {
        Ops::ShowOverlays(!ui.showOverlays);
      }
      break;
    case GLFW_KEY_HOME:
      if (!mods) {
        Ops::AlignViewportToOutput();
        break;
      } else if (mods == (GLFW_MOD_CONTROL | GLFW_MOD_ALT)) {
        Ops::AlignOutputToViewport();
        break;
      }
  }
}

void UI::ToggleTimelinePlay() {
  Timeline& timeline = App::GetInstance().timeline;
  if (timeline.IsPlaying()) {
    timeline.Pause();
  } else {
    timeline.Play();
  }
}

void UI::ToggleNavigationMode() {
  navigationMode = navigationMode == NavigationMode::Normal
                       ? NavigationMode::Walk
                       : NavigationMode::Normal;

  if (navigationMode == NavigationMode::Walk) {
    glfwSetCursorPos(window, (float)vW / 2, (float)vH / 2);
  }
}
}  // namespace UI
