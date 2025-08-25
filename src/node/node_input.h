#pragma once

#include <string>
#include <variant>

namespace node {

enum class InputType { File, Float };

using InputValue = std::variant<std::string, float>;

class Input {
 public:
  InputType type;
  InputValue value;
  std::string name;

  Input();
  Input(InputType type, std::string name);
  Input(InputType type, std::string name, InputValue value);

  template <typename T>
  T& Value() {
    return std::get<T>(value);
  }
};

}  // namespace node
