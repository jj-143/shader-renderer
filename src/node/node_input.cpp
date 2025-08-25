#include "node.h"

namespace node {

Input::Input() : Input(InputType::Float, "") {}

Input::Input(InputType type, std::string name, InputValue value)
    : type(type), value(value), name(name) {}

Input::Input(InputType type, std::string name) : type(type), name(name) {
  switch (type) {
    case InputType::Float:
      value = 0.0f;
      break;
    case InputType::File:
      value = "";
      break;
  }
}

}  // namespace node