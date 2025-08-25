#include <format>

#include "app.h"
#include "ops.h"
#include "timeline.h"
#include "ui.h"

namespace ui {

// static function [GLFWkeyfun] for relaying
void UI::OnKeyPressed([[maybe_unused]] GLFWwindow* window, int key,
                      [[maybe_unused]] int scancode, int action, int mods) {
  if (action != GLFW_PRESS) return;
  UI& ui = app::GetInstance().ui;
  switch (key) {
    case GLFW_KEY_TAB:
      if (!ui.isViewportFocused) break;
      ui.ToggleCameraMode();
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
        ops::OpenOpenProjectDialog();
      }
      break;
    case GLFW_KEY_R:
      if (mods == GLFW_MOD_CONTROL) {
        ops::ReloadProject();
      }
      break;
    case GLFW_KEY_S:
      if (mods == GLFW_MOD_CONTROL) {
        ops::SaveCurrentProject();
        break;
      } else if (mods == (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT)) {
        ops::OpenSaveAsDialog();
        break;
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
  Timeline& timeline = app::GetInstance().timeline;
  if (timeline.IsPlaying()) {
    timeline.Pause();
  } else {
    timeline.Play();
  }
}

void UI::ToggleCameraMode() {
  camera->mode =
      camera->IsNormalMode() ? Camera::Mode::Walk : Camera::Mode::Normal;

  if (camera->IsWalkMode()) {
    glfwSetCursorPos(window, (float)vW / 2, (float)vH / 2);
  }
}

}  // namespace ui
