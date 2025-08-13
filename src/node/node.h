#pragma once

#include <memory>
#include <string>

#include "gl.h"

namespace renderer {
struct Context;
}

namespace node {

class Node {
 public:
  GLuint input;
  GLuint output;

  bool isValid = false;

  virtual ~Node() = default;

  inline virtual std::shared_ptr<Node> Clone() {
    return std::make_shared<Node>(*this);
  }

  virtual void Init() {}

  virtual void Execute([[maybe_unused]] renderer::Context& ctx) {}

  virtual void SetSize([[maybe_unused]] int width,
                       [[maybe_unused]] int height) {}

  virtual void SetProgramPath([[maybe_unused]] const std::string& path) {}

  virtual void Validate([[maybe_unused]] renderer::Context& ctx) {}

 protected:
  int width;
  int height;
};

}  // namespace node