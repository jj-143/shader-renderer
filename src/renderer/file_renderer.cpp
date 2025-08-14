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

FileRenderer::~FileRenderer() { delete[] data; }

void FileRenderer::Setup(const FileRendererParams params, const Camera& camera,
                         GLFWwindow* share) {
  // Create invisible Window for new GL Context
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  window = ui::InitWindow(1, 1, "Render Result", share);

  // Init renderer for output
  renderer.Init(params.width, params.height);
  renderer.SetComputeShader(params.shaderPath.c_str());
  renderer.compositor.Validate();
  renderer.camera = camera;

  data = new GLubyte[params.width * params.height * 4];
  output = {.width = params.width, .height = params.height};
}

void FileRenderer::Teardown() { glfwDestroyWindow(window); }

void FileRenderer::Render(float iTime) {
  renderer.Render(iTime);
  GetTextureData(data);
}

bool FileRenderer::WriteToFile(const char* filename) {
  return WriteToPNG(filename, data, output.width, output.height);
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
