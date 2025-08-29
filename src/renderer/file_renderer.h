#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "app.h"
#include "compositor.h"
#include "error.h"
#include "gl.h"
#include "renderer.h"
#include "shader_manager.h"

namespace output {

struct FileRendererParams {
  ShaderManager& shaderManager;
  renderer::Compositor& compositor;
  std::unordered_map<std::string, node::Input>& uniforms;
  int width;
  int height;
};

class FileRenderer {
 public:
  renderer::Renderer renderer;

  std::optional<std::vector<error::Error>> Setup(
      const FileRendererParams params, const Camera& camera,
      GLFWwindow& renderWindow);

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

  std::unique_ptr<GLubyte[]> data;
  std::shared_ptr<GLFWwindow> window;
  Output output;
};

}  // namespace output
