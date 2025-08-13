#pragma once

#include <memory>

#include "camera.h"
#include "compositor.h"
#include "gl.h"
#include "render_context.h"

class ShaderManager;

namespace renderer {

class Renderer {
 public:
  std::shared_ptr<Context> ctx;

  Camera camera;
  Compositor compositor;

  GLuint* renderTexture;

  void Init(int width, int height);
  void InitContext(ShaderManager& shaderManager);
  void CopyCompositor(const Compositor& target);
  void Render(float iTime);
  void SetSize(int width, int height);

 private:
  int width;
  int height;
};

}  // namespace renderer