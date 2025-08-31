#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gl.h"
#include "node_input.h"

struct Shader;

namespace renderer {
struct Context;
class Compositor;
}  // namespace renderer

namespace node {

struct NodeInfo {
  std::string name;
  std::string label;
  std::vector<Input> inputs;
  std::vector<Input> uniforms;

  // states
  bool active = true;
  bool initialized = true;

  Input* GetInput(const std::string& name);

  Input* GetUniform(const std::string& name);
};

class Node : public NodeInfo {
 public:
  bool toRemove = false;
  bool isValid = false;
  std::weak_ptr<renderer::Compositor> compositor;

  Node(const std::string& name, const std::string& label = "")
      : NodeInfo(name, label) {};

  virtual ~Node() = default;

  virtual const NodeInfo& GetSpec() const = 0;

  virtual void Init() {}

  virtual void Execute([[maybe_unused]] renderer::Context& ctx) {}

  virtual void Validate([[maybe_unused]] renderer::Context& ctx) {}

  void OnInputChange(Input& input, const InputValue& value);

  void OnActiveChanged();

  void OnUniformChanged();

  /**
   * Acknowlege use of shaders in a Node.
   * Should be called in Validate()
   *
   * TODO: Delegate to Compositor or ContextManager
   * Shader usages can be gathered during Compositor::Validate() or in
   * ContextManger.
   */
  void RegisterShaders(std::vector<std::shared_ptr<Shader>> shaders);

  /// Update locations for registered shaders in this Node
  void UpdateUniformLocations();

  void UseShader(renderer::Context& ctx, std::shared_ptr<Shader> shader);

  // Modifying uniforms in runtime
  void AddUniform(node::Input uniform);

  void EditUniform(node::Input& target, node::Input newUniform);

  void RemoveUniform(node::Input& target);

  friend class renderer::Compositor;

 private:
  std::vector<std::shared_ptr<Shader>> shaders;
};

}  // namespace node