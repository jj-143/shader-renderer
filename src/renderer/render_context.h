#pragma once

#include <unordered_map>
#include <vector>

#include "camera.h"

class ShaderManager;
struct Shader;

namespace node {
class Node;
class Input;
}  // namespace node

namespace renderer {

class Compositor;

struct Context {
  Compositor& compositor;
  ShaderManager& shaderManager;
  Camera* camera;

  bool rendered = false;
  bool forceRender = false;

  float iTime;

  Context(Compositor& c, ShaderManager& s, Camera* cam)
      : compositor(c), shaderManager(s), camera(cam) {}

  friend class node::Node;

 private:
  void UseShader(Shader& shader, std::vector<node::Input>& uniforms);
};

}  // namespace renderer