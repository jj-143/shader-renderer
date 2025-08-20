#pragma once

#include <memory>
#include <string>

#include "gl.h"

namespace renderer {
struct Context;
}

namespace node {

class ShaderNode {
 public:
  GLuint input;
  GLuint output;

  bool isValid = false;

  virtual ~ShaderNode() = default;

  inline virtual std::shared_ptr<ShaderNode> Clone() {
    return std::make_shared<ShaderNode>(*this);
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