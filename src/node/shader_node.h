#pragma once

#include <memory>
#include <string>

#include "gl.h"
#include "node.h"

namespace node {

class ShaderNode : public Node {
 public:
  GLuint input;
  GLuint output;

  bool isValid = false;

  ShaderNode(const std::string& name, const std::string& label = "")
      : Node(name, label) {}

  inline const NodeInfo& GetSpec() const override {
    static NodeInfo empty;
    return empty;
  }

  inline virtual std::shared_ptr<ShaderNode> Clone() {
    return std::make_shared<ShaderNode>(*this);
  }

  virtual void SetSize([[maybe_unused]] int width,
                       [[maybe_unused]] int height) {}

 protected:
  int width;
  int height;
};

}  // namespace node