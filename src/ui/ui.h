#pragma once
#include <imgui/imgui.h>

#include <optional>
#include <string>

#include "camera.h"
#include "gl.h"

class Camera;

namespace ui::colors {

inline const ImVec4 SHADE_2 = ImColor(56, 85, 85, 255);
inline const ImVec4 SHADE_3 = ImColor(48, 66, 66, 255);
inline const ImVec4 SHADE_4 = ImColor(32, 58, 58, 255);
inline const ImVec4 SHADE_5 = ImColor(24, 39, 39, 255);
inline const ImVec4 SHADE_6 = ImColor(8, 27, 28, 255);

}  // namespace ui::colors

namespace ui {

enum class NavigationMode { Normal, Walk };

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

inline const ImVec4   BG_COLOR           = colors::SHADE_6;
inline const ImVec4   TEXT_COLOR         = ImColor(181, 183, 188, 255);
inline const ImVec4   GAP_COLOR          = BG_COLOR;
inline const ImVec4   PANEL_BG_COLOR     = colors::SHADE_5;
inline const ImVec4   STATUS_BAR_COLOR   = BG_COLOR;

inline const ImVec4   MENU_HOVERED       = colors::SHADE_5;
inline const ImVec4   MENU_SELECTED      = colors::SHADE_3;
inline const ImVec4   ITEM_HOVERED       = colors::SHADE_4;
inline const ImVec4   ITEM_ACTIVE        = colors::SHADE_2;

inline const float    STATUS_DURATION    = 3;

inline const float    SLIDER_SPEED       = 0.2f;
// clang-format on

class UI {
 public:
  int wW, wH;  // window
  int vW, vH;  // viewport
  GLFWwindow* window;

  bool showOverlays = true;
  bool isViewportFocused = false;
  bool isViewportMaximized = false;
  bool shouldFocusViewport = true;

  NavigationMode navigationMode = NavigationMode::Normal;

  std::optional<std::string> errorLog;

  GLuint* viewportTextureID;

  bool InitUI(const int width, const int height, const char* title);
  void Terminate();
  bool NewFrame();
  void Startup();
  void Render();

  void MaximizeViewport(bool set);
  void SetCamera(Camera& camera) { this->camera = &camera; }

  void UpdateCameraControl();
  void UpdateStatus(const std::string message);
  void Quit();
  void OnWindowResize(int width, int height);
  void RenderSidePanel();

 private:
  Camera* camera;
  std::string statusMessage;
  float statusClearTime;

  void HandleFocusViewport();

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

ImVec2 CalculateWindowSize(ImVec2 viewport);
GLFWwindow* InitWindow(const int width, const int height, const char* title,
                       GLFWwindow* share = nullptr);

}  // namespace ui