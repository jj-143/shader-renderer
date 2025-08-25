#pragma once

#include <optional>

#include "node.h"
#include "node_input.h"

namespace ui::editor {

void RenderNodeInput(node::Node& node, node::Input& input,
                     bool isUniform = false);

}  // namespace ui::editor
