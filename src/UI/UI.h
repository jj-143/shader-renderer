#pragma once
#include <imgui/imgui.h>

#include <string>

#include "../Renderer/Camera.h"
#include "../common.h"

class Camera;

namespace UI {
enum class NavigationMode { NORMAL, WALK };

// clang-format off
inline const ImVec2   INSET              = {0, 0};
inline const ImVec2   LAYOUT_INSET       = {8, 8};
inline const int      LINE_HEIGHT        = 13;
inline const int      INNER_GAP          = 8;
inline const int      ROUNDING           = 8;
inline const ImVec2   MENU_BAR_PADDING   = {12, 12};
inline const float    MENU_BAR_HEIGHT    = LINE_HEIGHT + MENU_BAR_PADDING.y * 2;
inline const ImVec2   MAIN_MARGIN        = {8, 0};
inline const ImVec2   SIDE_PANEL_PADDING = {12, 12};
inline const int      SIDE_PANEL_WIDTH   = 300;
inline const ImVec2   STATUS_BAR_PADDING = {16, 8};
inline const int      STATUS_BAR_HEIGHT  = LINE_HEIGHT + STATUS_BAR_PADDING.y * 2;

inline const ImVec4   BG_COLOR           = ImColor(  8,  27,  28, 255);
inline const ImVec4   TEXT_COLOR         = ImColor(181, 183, 188, 255);
inline const ImVec4   GAP_COLOR          = ImColor(  8,  27,  28, 255);
inline const ImVec4   MENU_HOVERED       = ImColor( 24,  39,  39, 255);
inline const ImVec4   PANEL_BG_COLOR     = ImColor( 24,  39,  39, 255);
inline const ImVec4   STATUS_BAR_COLOR   = ImColor(  8,  27,  28, 255);

inline const float    STATUS_DURATION    = 3;
// clang-format on

class UI {
 public:
  int wW, wH;  // window
  int vW, vH;  // viewport
  GLFWwindow* window;

  bool showDebugStat = true;
  NavigationMode navigationMode = NavigationMode::NORMAL;

  GLuint viewportTextureID;

  bool InitUI(const int width, const int height, const char* title);
  void Terminate();
  bool NewFrame();
  void Render();
  void SetCamera(Camera& camera) { this->camera = &camera; }
  void UpdateCameraControl();
  void UpdateStatus(const std::string message);

 private:
  Camera* camera;
  std::string statusMessage;
  float statusClearTime;

  void PopGlobalStyles();
  void PushGlobalStyles();
  void RenderMain();
  void RenderMainMenu();
  void RenderStatusBar();
  void RenderViewport();

  // Shortcuts
  static void OnKeyPressed(GLFWwindow* window, int key, int scancode,
                           int action, int mods);
  void ToggleTimelinePlay();
  void ToggleNavigationMode();
};

void CalculateWindowSize(const int& vW, const int& vH, int& wW, int& wH);
GLFWwindow* CreateWindow(const int width, const int height, const char* title);
}  // namespace UI