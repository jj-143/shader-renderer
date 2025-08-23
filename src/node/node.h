#pragma once

#include <memory>
#include <string>

#include "gl.h"

namespace renderer {
struct Context;
}

namespace node {

struct NodeInfo {
  std::string name;
  std::string label;
  std::string shaderPath;

  // states
  bool initialized = true;
};

class Node : public NodeInfo {
 public:
  bool toRemove = false;

  Node(const std::string& name, const std::string& label = "")
      : NodeInfo({name, label, ""}) {};

  virtual ~Node() = default;

  virtual const NodeInfo& GetSpec() const = 0;

  virtual void Init() {}

  virtual void Execute([[maybe_unused]] renderer::Context& ctx) {}

  virtual void Validate([[maybe_unused]] renderer::Context& ctx) {}

  void OnShaderFileChanged(const std::string& shaderPath);
};

}  // namespace node