#pragma once

#include "app.h"
#include "gl.h"
#include "renderer.h"

namespace output {

struct FileRendererParams {
  std::string shaderPath;
  int width;
  int height;
};

class FileRenderer {
 public:
  renderer::Renderer renderer;

  ~FileRenderer();

  void Setup(const FileRendererParams params, const Camera& camera,
             GLFWwindow* window);

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

}  // namespace output
