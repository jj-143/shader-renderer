#pragma once

#include <string>

namespace renderer {
struct Context;
}

namespace node {

class Node {
 public:
  GLuint input;
  GLuint output;

  bool isValid = false;
  std::string errorLog = "";

  virtual ~Node() = default;

  virtual void Init() {}

  virtual void Execute([[maybe_unused]] renderer::Context& ctx) {}

  virtual void SetSize([[maybe_unused]] int width,
                       [[maybe_unused]] int height) {}

  virtual void SetProgramPath([[maybe_unused]] const std::string& path) {}

  virtual void Validate() {}

 protected:
  int width;
  int height;
};

}  // namespace node