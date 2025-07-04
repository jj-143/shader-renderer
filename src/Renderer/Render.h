#pragma once

#include "Renderer.h"
#include "app.h"
#include "common.h"

namespace Render {
struct Params {
  std::string shaderPath;
  int width;
  int height;
};

class RenderContext {
 public:
  Renderer renderer;

  ~RenderContext();

  void Setup(const Params params, const Camera& camera);

  void Teardown();

  /// Render current scene to internal buffer.
  void Render(float iTime);

  /// Write internal buffer to a file.
  /// @return true if success
  bool WriteToFile(const char* filename);

 private:
  struct Output {
    int width;
    int height;
  };

  GLubyte* data;
  GLFWwindow* window;
  Output output;
};
}  // namespace Render
