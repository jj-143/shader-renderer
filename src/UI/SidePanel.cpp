#include "SidePanel.h"

#include <algorithm>
#include <array>

#include "../Timeline.h"
#include "../app.h"
#include "UI.h"

namespace {
using namespace UI;
struct FrameRateItem {
  const float value;
  const char* label;
};
const int LABEL_WIDTH = 100;
const int CONTROL_WIDTH =
    SIDE_PANEL_WIDTH - SIDE_PANEL_PADDING.x * 2 - LABEL_WIDTH;
const int CONTROL_START_X = LABEL_WIDTH + SIDE_PANEL_PADDING.x;

std::array<FrameRateItem, 3> FRAMERATE_ITEMS = {
    FrameRateItem{24, "24"}, FrameRateItem{30, "30"}, FrameRateItem{60, "60"}};

void FrameSettings();
void OutputSettings();
void DevPanel();
void Help();
}  // namespace

namespace UI::SidePanel {
void RenderSidePanel() {
  // Panel Container
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, SIDE_PANEL_PADDING);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, PANEL_BG_COLOR);
  ImGui::BeginChild("SidePanel", ImVec2(SIDE_PANEL_WIDTH, 0),
                    ImGuiChildFlags_FrameStyle);
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor();
  // END Panel Container

  // Panel Contents
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8, 4});
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});

  {
    FrameSettings();
    OutputSettings();
    Help();
#ifndef NDEBUG
    DevPanel();
#endif
  }
  ImGui::PopStyleVar(3);
  // END Panel Contents

  ImGui::EndChild();
}
}  // namespace UI::SidePanel

// Locals
namespace {
using namespace UI;
void DevPanel() {
  App& app = App::GetInstance();
  Timeline& timeline = app.timeline;

  ImGui::SeparatorText("Dev Panel");

  if (ImGui::Button(timeline.IsPlaying()   ? "PLAYING"
                    : timeline.IsPaused()  ? "PAUSED"
                    : timeline.IsStopped() ? "STOPPED"
                                           : "")) {
    if (timeline.IsPlaying()) {
      timeline.Pause();
    } else {
      timeline.Play();
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("STOP")) {
    timeline.Stop();
  }
}

void Help() {
  ImGui::SeparatorText("Help");

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {4, 8});

  ImGui::Text("Navigation");
  ImGui::BulletText("Toggle [Walk, Normal] (Tab)");
  ImGui::BulletText("WASD + Mouse to navigate in Walk Mode");

  ImGui::Spacing();

  ImGui::Text("Timeline");
  ImGui::BulletText("Play/Pause (Space)");

  ImGui::PopStyleVar(2);
}

void FrameSettings() {
  const float dragSpeed = .2f;
  Output& output = App::GetInstance().setting.output;
  ImGui::SeparatorText("Frame");

  {
    // Frame Start
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Frame Start");
    ImGui::SameLine(CONTROL_START_X);
    ImGui::SetNextItemWidth(CONTROL_WIDTH);
    ImGui::DragScalar("##frameStart", ImGuiDataType_U16, &output.frameStart,
                      dragSpeed, nullptr, &output.frameEnd, "%u");
  }

  {
    // Frame End
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Frame End");
    ImGui::SameLine(CONTROL_START_X);
    ImGui::SetNextItemWidth(CONTROL_WIDTH);
    ImGui::DragScalar("##frameEnd", ImGuiDataType_U16, &output.frameEnd,
                      dragSpeed, &output.frameStart, nullptr, "%u");
  }

  {
    // Frame Rate
    static int index =
        std::find_if(
            FRAMERATE_ITEMS.begin(), FRAMERATE_ITEMS.end(),
            [&](FrameRateItem it) { return it.value == output.frameRate; }) -
        FRAMERATE_ITEMS.begin();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Frame Rate");
    ImGui::SameLine(CONTROL_START_X);
    ImGui::SetNextItemWidth(CONTROL_WIDTH);
    if (ImGui::Combo(
            "##frameRate", &index,
            [](void* it, int i) { return ((const FrameRateItem*)it)[i].label; },
            FRAMERATE_ITEMS.data(), FRAMERATE_ITEMS.size())) {
      output.frameRate = FRAMERATE_ITEMS[index].value;
    }
  }
}

void OutputSettings() {
  Output& output = App::GetInstance().setting.output;
  ImGui::SeparatorText("Output");

  {
    // Path
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Path");
    ImGui::SameLine(CONTROL_START_X);
    ImGui::SetNextItemWidth(CONTROL_WIDTH);
    ImGui::InputText("##outputPath", output.path, IM_ARRAYSIZE(output.path));
  }

  {
    // Format
    static int index =
        std::find_if(
            SUPPORTED_FORMATS.begin(), SUPPORTED_FORMATS.end(),
            [&](Format f) { return !strcmp(f.ext, output.format.ext); }) -
        SUPPORTED_FORMATS.begin();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Format");
    ImGui::SameLine(CONTROL_START_X);
    ImGui::SetNextItemWidth(CONTROL_WIDTH);
    if (ImGui::Combo(
            "##format", &index,
            [](void* format, int i) {
              return ((const Format*)format)[i].label;
            },
            SUPPORTED_FORMATS.data(), SUPPORTED_FORMATS.size())) {
      output.format = SUPPORTED_FORMATS[index];
    }
  }
}

}  // namespace
