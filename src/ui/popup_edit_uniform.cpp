#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include <optional>

#include "editor_node_input.h"
#include "logger.h"
#include "node.h"
#include "node_input.h"
#include "ops.h"
#include "popup.h"

namespace {

using UniformOption = std::pair<const char*, node::InputType>;

auto UNIFORM_OPTIONS = std::array<UniformOption, 1>{
    UniformOption{"float", node::InputType::Float},
};

struct EditUniformState {
  std::string name = "";
  node::InputType type = UNIFORM_OPTIONS[0].second;
  int _uniformType_index = 0;
};

struct EditUniformDialogResult {
  bool isRemove = false;
  node::Input input = {};
};

const int LABEL_WIDTH = 48;
const int FIELD_START = LABEL_WIDTH + 8;
const int FIELD_WIDTH = 140;

std::optional<EditUniformDialogResult> OpenPopup(node::Input* input);

void AddUniformEx(node::Node* node, node::Input& newInput);

void EditUniformEx(node::Node* node, node::Input* input, node::Input& newInput);

void RemoveUniformEx(node::Node* node, node::Input* input);

}  // namespace

namespace ui::popup {

void EditUniform(node::Node& node, node::Input* input) {
  auto result = OpenPopup(input);
  if (!result) return;

  // Edit uniform
  if (input != nullptr) {
    if (result->isRemove) {
      RemoveUniformEx(&node, input);
    } else {
      EditUniformEx(&node, input, result->input);
    }
  } else {
    AddUniformEx(&node, result->input);
  }
}

}  // namespace ui::popup

namespace {

using namespace ui;

/// Populate state from `input` (if it's editing)
EditUniformState InitState(node::Input* input) {
  if (!input) return {};

  /**
   * NOTE: _uniformType_index is only for ImGui::Combo internal,
   * ultimately for displaying label, and set the correct type when
   * selecting any.
   *
   * It's okay if user confirms while index=-1, since state.type is set
   * from input.type on mount.
   *
   * And if user select an option, the value is in the range.
   */
  int uniformType_index = -1;

  if (auto it = std::ranges::find_if(UNIFORM_OPTIONS,
                                     [&input](UniformOption& option) {
                                       return option.second == input->type;
                                     });
      it != UNIFORM_OPTIONS.end()) {
    uniformType_index = std::distance(UNIFORM_OPTIONS.begin(), it);
  }

  return EditUniformState{
      .name = input->name,
      .type = input->type,
      ._uniformType_index = uniformType_index,
  };
}

std::optional<EditUniformDialogResult> OpenPopup(node::Input* input) {
  if (!ImGui::BeginPopup(popup::EDIT_UNIFORM)) return std::nullopt;
  const bool isEditing = input != nullptr;
  const bool showRemove = isEditing;

  const char* textTitle = isEditing ? "Edit Uniform" : "Add New Uniform";
  const char* textConfirm = isEditing ? "Edit" : "Add";
  const char* textRemove = "Remove";

  static EditUniformState state;

  if (popup::OnMount(popup::EDIT_UNIFORM)) {
    state = InitState(input);
  }

  {
    // Popup Title
    ImGui::TextColored(colors::SHADE_2, "%s", textTitle);

    ImGui::Spacing();

    // Uniform name
    {
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Name");

      ImGui::SameLine(FIELD_START);

      ImGui::SetNextItemWidth(FIELD_WIDTH);
      ImGui::InputTextWithHint("##UniformName", "name", &state.name);
    }

    // Uniform Type
    {
      ImGui::AlignTextToFramePadding();
      ImGui::Text("Type");

      ImGui::SameLine(FIELD_START);

      ImGui::SetNextItemWidth(FIELD_WIDTH);

      if (ImGui::Combo(
              "##EditUniformType", &state._uniformType_index,
              [](void* it, int idx) {
                return ((const UniformOption*)it)[idx].first;
              },
              UNIFORM_OPTIONS.data(), UNIFORM_OPTIONS.size())) {
        state.type = UNIFORM_OPTIONS[state._uniformType_index].second;
        logger::Debug("Selected: {}", state._uniformType_index);
      }
    }

    // Confirm button
    {
      bool disabled = state.name.empty();

      ImGui::BeginDisabled(disabled);

      bool clicked = ImGui::Button(textConfirm, {-1, 0});

      ImGui::EndDisabled();

      if (clicked) {
        ImGui::CloseCurrentPopup();
        ImGui::EndPopup();

        return EditUniformDialogResult{
            .isRemove = false,
            .input = node::Input{state.type, state.name},
        };
      }
    }

    // Confirm button
    if (showRemove) {
      if (ImGui::Button(textRemove, {-1, 0})) {
        ImGui::CloseCurrentPopup();
        ImGui::EndPopup();

        return EditUniformDialogResult{
            .isRemove = true,
            // discarded
            .input = node::Input{},
        };
      }
    }

    ImGui::EndPopup();
  }

  return std::nullopt;
}

void AddUniformEx(node::Node* node, node::Input& uniform) {
  ops::DeferUITask([node, uniform] {
    if (!node) return;
    node->AddUniform(uniform);
  });
}

void EditUniformEx(node::Node* node, node::Input* old,
                   node::Input& newUniform) {
  ops::DeferUITask([node, old, newUniform] {
    if (!node || !old) return;
    node->EditUniform(*old, newUniform);
  });
}

void RemoveUniformEx(node::Node* node, node::Input* uniform) {
  ops::DeferUITask([node, target = uniform] {
    if (!node || !target) return;
    node->RemoveUniform(*target);
  });
}

}  // namespace