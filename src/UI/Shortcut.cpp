#include "../Core/Ops.h"
#include "../Timeline.h"
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
      ui.ToggleNavigationMode();
      break;
    case GLFW_KEY_SPACE:
      ui.ToggleTimelinePlay();
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
  navigationMode = navigationMode == NavigationMode::NORMAL
                       ? NavigationMode::WALK
                       : NavigationMode::NORMAL;

  if (navigationMode == NavigationMode::WALK) {
    glfwSetCursorPos(window, (float)vW / 2, (float)vH / 2);
  }
}
}  // namespace UI
