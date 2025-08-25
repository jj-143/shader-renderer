#include "popup.h"

#include <imgui/imgui.h>

#include <unordered_set>

/**
 * This allows to run codes only once on after it's triggered to be opened,
 * inside a ImGui::Popup. Can be used with ImGui::Modal, with some refactor.
 *
 * EXAMPLE:
 * In 'popup_edit_uniform', it keeps a local editing copy of node::Input,
 * which is initializes to editing target or a default one, which persist
 * while the popup is being opened.
 */

namespace {

std::unordered_set<const char*> onMountFlag;

}  // namespace

namespace ui::popup {

void Open(const char* id) {
  ImGui::OpenPopup(id);
  onMountFlag.emplace(id);
}

bool OnMount(const char* id) {
  bool onMount = onMountFlag.contains(id);
  onMountFlag.erase(id);
  return onMount;
}

}  // namespace ui::popup
