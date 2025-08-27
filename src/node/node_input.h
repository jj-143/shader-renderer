#pragma once

#include <glm/glm.hpp>
#include <string>
#include <variant>

namespace node {

enum class InputType { File, Float, Mat4 };

using InputValue = std::variant<std::string, float, glm::mat4>;

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
