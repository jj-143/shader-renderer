#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gl.h"
#include "node_input.h"

namespace renderer {
struct Context;
}

namespace node {

struct NodeInfo {
  std::string name;
  std::string label;
  std::vector<Input> inputs;

  // states
  bool active = true;
  bool initialized = true;

  Input* GetInput(const std::string& name);
};

class Node : public NodeInfo {
 public:
  bool toRemove = false;
  bool isValid = false;

  Node(const std::string& name, const std::string& label = "")
      : NodeInfo(name, label) {};

  virtual ~Node() = default;

  virtual const NodeInfo& GetSpec() const = 0;

  virtual void Init() {}

  virtual void Execute([[maybe_unused]] renderer::Context& ctx) {}

  virtual void Validate([[maybe_unused]] renderer::Context& ctx) {}

  void OnInputChange(Input& input, const InputValue& value);

  void OnActiveChanged();
};

}  // namespace node