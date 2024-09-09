#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "Postprocess.h"
#include "ShaderLoader.h"

void Renderer::Init(int w, int h, GLenum colorbufferFormat) {
  this->w = w;
  this->h = h;
  colorbuffer = Postprocess::CreateColorbuffer(w, h, colorbufferFormat);
  camera = Camera();
  initialized = true;
}

void Renderer::Render(float iTime) {
  switch (shaderType) {
    case COMPUTE_SHADER:
      RenderWithComputeShader(iTime);
      break;
    default:
      break;
  }
}

ShaderCompileResult Renderer::SetComputeShader(const char* path) {
  assert(initialized);
  ShaderCompileResult result = MakeComputeShader(path);
  if (!result.isSuccess) return result;

  shader = result.program;
  shaderType = COMPUTE_SHADER;
  workgroupCountX = (w + 7) / 8;
  workgroupCountY = (h + 7) / 8;
  viewLocation = glGetUniformLocation(shader, "view");
  iTimeLocation = glGetUniformLocation(shader, "iTime");
  return result;
}

void Renderer::DeleteShader() {
  glDeleteProgram(shader);  //
}

void Renderer::RenderWithComputeShader(float iTime) {
  glUseProgram(shader);
  glUniform1f(iTimeLocation, iTime);
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.view));
  glBindImageTexture(0, colorbuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
  glDispatchCompute(workgroupCountX, workgroupCountY, 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
