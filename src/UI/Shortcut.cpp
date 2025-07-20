#include <format>

#include "Ops.h"
#include "Timeline.h"
#include "UI.h"
#include "app.h"

namespace ui {

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
        ops::MaximizeViewport(!ui.isViewportMaximized);
        break;
      }
      if (ui.isViewportFocused) {
        ui.ToggleTimelinePlay();
        break;
      }
      break;
    case GLFW_KEY_ESCAPE:
      ops::CancelTask();
      break;
    case GLFW_KEY_F12:
      if (mods == GLFW_MOD_CONTROL) {
        ops::Render(true);
      } else {
        ops::Render(false);
      }
      break;
    case GLFW_KEY_G:
      if (mods == GLFW_MOD_ALT) {
        ops::ResetCamera();
      }
      break;
    case GLFW_KEY_O:
      if (mods == GLFW_MOD_CONTROL) {
        ops::OpenOpenShaderDialog();
      }
      break;
    case GLFW_KEY_R:
      if (mods == GLFW_MOD_CONTROL) {
        ops::ReloadShader();
      }
      break;
    case GLFW_KEY_Q:
      if (mods == GLFW_MOD_CONTROL) {
        ops::Quit();
      }
      break;
    case GLFW_KEY_Z:
      if (mods == (GLFW_MOD_ALT | GLFW_MOD_SHIFT)) {
        ops::ShowOverlays(!ui.showOverlays);
      }
      break;
    case GLFW_KEY_HOME:
      if (!mods) {
        ops::AlignViewportToOutput();
        break;
      } else if (mods == (GLFW_MOD_CONTROL | GLFW_MOD_ALT)) {
        ops::AlignOutputToViewport();
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

}  // namespace ui
