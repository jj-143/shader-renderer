#include "file_renderer.h"

#include <stb/stb_write.h>

#include <string>

#include "app.h"
#include "ui.h"

namespace {

bool WriteToPNG(const char* filename, void* data, const int& width,
                const int& height);
void GetTextureData(GLubyte* data);

}  // namespace

namespace output {

std::optional<std::vector<error::Error>> FileRenderer::Setup(
    const FileRendererParams params, const Camera& camera, GLFWwindow* share) {
  CreateWindow(share);

  // Init renderer for output
  renderer.Init(params.width, params.height);
  renderer.InitContext(params.shaderManager);
  renderer.CopyCompositor(params.compositor.Clone());

  std::vector<error::Error> errors;
  renderer.compositor.Validate(*renderer.ctx, errors);
  params.shaderManager.Refresh(errors);

  if (errors.size()) return errors;

  renderer.camera = camera;

  data = std::make_unique<GLubyte[]>(params.width * params.height * 4);
  output = {.width = params.width, .height = params.height};

  return std::nullopt;
}

void FileRenderer::Render(float iTime) {
  renderer.Render(iTime);
  GetTextureData(data.get());
}

bool FileRenderer::WriteToFile(const char* filename) {
  return WriteToPNG(filename, data.get(), output.width, output.height);
}

void FileRenderer::CreateWindow(GLFWwindow* share) {
  // Create invisible Window for new GL Context
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  auto deleter = [](GLFWwindow* ptr) { glfwDestroyWindow(ptr); };

  window = std::shared_ptr<GLFWwindow>(
      ui::InitWindow(1, 1, "Render Result", share), deleter);
}

}  // namespace output

namespace {

bool WriteToPNG(const char* filename, void* data, const int& width,
                const int& height) {
  const int comp = 4;  // "RGBA". see: "stb_image_write"
  const int result =
      stbi_write_png(filename, width, height, comp, data, 4 * width);
  return result != 0;  // 0 for Error
}

void GetTextureData(GLubyte* data) {
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

}  // namespace
