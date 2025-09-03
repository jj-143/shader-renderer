#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "app.h"
#include "side_panel.h"
#include "ui.h"

namespace {

void FrameSettings();
void OutputSettings();

}  // namespace

namespace ui::side_panel {

void Settings() {
  {
    FrameSettings();
    OutputSettings();
  }
}

}  // namespace ui::side_panel

namespace {

struct FrameRateItem {
  const float value;
  const char* label;
};

std::array<FrameRateItem, 3> FRAMERATE_ITEMS = {
    FrameRateItem{24, "24"}, FrameRateItem{30, "30"}, FrameRateItem{60, "60"}};

void FrameSettings() {
  Output& output = app::GetInstance().setting.output;
  ImGui::SeparatorText("Frame");

  {
    // Frame Start
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Frame Start");
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
    ImGui::DragScalar("##frameStart", ImGuiDataType_U16, &output.frameStart,
                      ui::SLIDER_SPEED_INT, nullptr, &output.frameEnd, "%u");
  }

  {
    // Frame End
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Frame End");
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
    ImGui::DragScalar("##frameEnd", ImGuiDataType_U16, &output.frameEnd,
                      ui::SLIDER_SPEED_INT, &output.frameStart, nullptr, "%u");
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
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
    if (ImGui::Combo(
            "##frameRate", &index,
            [](void* it, int i) { return ((const FrameRateItem*)it)[i].label; },
            FRAMERATE_ITEMS.data(), FRAMERATE_ITEMS.size())) {
      output.frameRate = FRAMERATE_ITEMS[index].value;
    }
  }
}

void OutputSettings() {
  Output& output = app::GetInstance().setting.output;
  ImGui::SeparatorText("Output");

  {
    // Resolution - Width
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Width");
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
    ImGui::DragScalar("##width", ImGuiDataType_U16, &output.width,
                      ui::SLIDER_SPEED_INT, nullptr, nullptr, "%u");
  }

  {
    // Resolution - Height
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Height");
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
    ImGui::DragScalar("##height", ImGuiDataType_U16, &output.height,
                      ui::SLIDER_SPEED_INT, nullptr, nullptr, "%u");
  }

  {
    // Path
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Path");
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##outputPath", &output.path);
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
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
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
