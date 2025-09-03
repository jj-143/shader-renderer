#pragma once

#include <functional>

#include "ui.h"

namespace ui::side_panel {

const float LABEL_WIDTH = 110;
const float FIELD_START = LABEL_WIDTH + SIDE_PANEL_PADDING.x;

void TabItem(const char* label, std::function<void()> Child);

void Settings();

void CompositorEditor();

void Help();

}  // namespace ui::side_panel
