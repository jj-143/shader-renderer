#pragma once
#include <imgui/imgui.h>

#include "Renderer/Camera.h"
#include "common.h"

class Camera;

// clang-format off
static const ImVec2   INSET              = {0, 0};
static const ImVec2   LAYOUT_INSET       = {8, 8};
static const int      LINE_HEIGHT        = 13;
static const int      INNER_GAP          = 8;
static const int      ROUNDING           = 8;
static const ImVec2   MENU_BAR_PADDING   = {12, 12};
static const float    MENU_BAR_HEIGHT    = LINE_HEIGHT + MENU_BAR_PADDING.y * 2;
static const ImVec2   MAIN_MARGIN        = {8, 0};
static const ImVec2   SIDE_PANEL_PADDING = {12, 12};
static const int      SIDE_PANEL_WIDTH   = 300;
static const ImVec2   STATUS_BAR_PADDING = {16, 8};
static const int      STATUS_BAR_HEIGHT  = LINE_HEIGHT + STATUS_BAR_PADDING.y * 2;

static const ImVec4   BG_COLOR           = ImColor(  8,  27,  28, 255);
static const ImVec4   TEXT_COLOR         = ImColor(181, 183, 188, 255);
static const ImVec4   GAP_COLOR          = ImColor(  8,  27,  28, 255);
static const ImVec4   MENU_HOVERED       = ImColor( 24,  39,  39, 255);
static const ImVec4   PANEL_BG_COLOR     = ImColor( 24,  39,  39, 255);
static const ImVec4   STATUS_BAR_COLOR   = ImColor(  8,  27,  28, 255);
// clang-format on

class UI {
 public:
  enum class NavigationMode { NORMAL, WALK };

  int wW, wH;  // window
  int vW, vH;  // viewport
  float iTime = 0;
  GLFWwindow* window;

  bool showDebugStat = true;
  NavigationMode navigationMode = NavigationMode::NORMAL;

  GLuint viewportTextureID;

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

  void PopGlobalStyles();
  void PushGlobalStyles();
  void RenderMain();
  void RenderMainMenu();
  void RenderSidePanel();
  void RenderStatusBar();
  void RenderViewport();

  static void CalculateWindowSize(const int& vW, const int& vH, int& wW,
                                  int& wH);
};
