#include "Render.h"

#include <stb/stb_write.h>

#include <string>

#include "../UI/UI.h"
#include "../app.h"

namespace {
bool WriteToPNG(const char* filename, void* data, const int& width,
                const int& height);
void GetTextureData(GLubyte* data);
}  // namespace

namespace Render {
RenderContext::~RenderContext() { delete[] data; }

void RenderContext::Setup(const App::Config& config, const Camera& camera) {
  // Create invisible Window for new GL Context
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  window = UI::InitWindow(1, 1, "Render Result");

  // Init renderer for output
  renderer.Init(config.vW, config.vH);
  renderer.SetComputeShader(config.shaderPath.c_str());
  renderer.camera = camera;

  data = new GLubyte[config.vW * config.vW * 4];
  output = {.width = config.vW, .height = config.vH};
}  // namespace Render

void RenderContext::Teardown() { glfwDestroyWindow(window); }

void RenderContext::Render(float iTime) {
  renderer.Render(iTime);
  GetTextureData(data);
}

bool RenderContext::WriteToFile(const char* filename) {
  return WriteToPNG(filename, data, output.width, output.height);
}
}  // namespace Render

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
