#pragma once

#include "camera.h"
#include "compositor.h"
#include "gl.h"
#include "render_context.h"

namespace renderer {

class Renderer {
 public:
  Context ctx;

  Camera camera;
  Compositor compositor;

  GLuint* renderTexture;

  void Init(int width, int height);
  void SetComputeShader(const char* path);
  void Render(float iTime);
  void SetSize(int width, int height);
};

}  // namespace renderer