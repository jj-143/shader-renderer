#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "camera.h"
#include "node_input.h"

class ShaderManager;
struct Shader;

namespace node {
class Node;
}  // namespace node

namespace renderer {

class Compositor;

struct Context {
  std::shared_ptr<Compositor> compositor;
  ShaderManager& shaderManager;
  Camera* camera;

  bool rendered = false;
  bool forceRender = false;

  std::unordered_map<std::string, node::Input> uniforms = {
      {"iTime", node::Input{node::InputType::Float, "iTime"}},
      {"view", node::Input{node::InputType::Mat4, "view"}},
  };

  Context(std::shared_ptr<Compositor> c, ShaderManager& s, Camera* cam)
      : compositor(c), shaderManager(s), camera(cam) {}

  friend class node::Node;

 private:
  void UseShader(Shader& shader, std::vector<node::Input>& uniforms);
};

}  // namespace renderer