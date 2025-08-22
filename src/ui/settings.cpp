#include "app.h"
#include "side_panel.h"
#include "ui.h"

namespace {

void FrameSettings();
void OutputSettings();
void Help();
void DevPanel();

}  // namespace

namespace ui::side_panel {

void Settings() {
  {
    FrameSettings();
    OutputSettings();
    Help();
#ifndef NDEBUG
    DevPanel();
#endif
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
                      ui::SLIDER_SPEED, nullptr, &output.frameEnd, "%u");
  }

  {
    // Frame End
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Frame End");
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
    ImGui::DragScalar("##frameEnd", ImGuiDataType_U16, &output.frameEnd,
                      ui::SLIDER_SPEED, &output.frameStart, nullptr, "%u");
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
                      ui::SLIDER_SPEED, nullptr, nullptr, "%u");
  }

  {
    // Resolution - Height
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Height");
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
    ImGui::DragScalar("##height", ImGuiDataType_U16, &output.height,
                      ui::SLIDER_SPEED, nullptr, nullptr, "%u");
  }

  {
    // Path
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Path");
    ImGui::SameLine(ui::side_panel::FIELD_START);
    ImGui::SetNextItemWidth(-1);
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

void DevPanel() {
  auto& app = app::GetInstance();
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

}  // namespace
