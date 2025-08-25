#pragma once

namespace node {
class Node;
class Input;
}  // namespace node

namespace ui::popup {

inline const char* EDIT_UNIFORM = "EDIT_UNIFORM";

void Open(const char* id);

bool OnMount(const char* id);

/// `EDIT_UNIFORM`
void EditUniform(node::Node& node, node::Input* input);

}  // namespace ui::popup
