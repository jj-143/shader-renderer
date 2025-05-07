#include "Renderer.h"

#include <format>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderLoader.h"

void Renderer::Init(int w, int h, GLenum colorbufferFormat) {
  format = colorbufferFormat;
  InitRenderTexture(colorbufferFormat);
  SetSize(w, h);
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

void Renderer::SetSize(int width, int height) {
  workgroupCountX = (width + 7) / 8;
  workgroupCountY = (height + 7) / 8;
  glBindTexture(GL_TEXTURE_2D, renderTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_FLOAT,
               NULL);
}

ShaderCompileResult Renderer::SetComputeShader(const char* path) {
  assert(initialized);
  ShaderCompileResult result = MakeComputeShader(path);

  if (!result.isSuccess) {
    state = State::COMPILE_ERROR;
    errorLog = std::format("{:s}\n\n", path);
    errorLog.append(result.error);
    return result;
  }

  state = State::RUNNING;
  errorLog = "";

  shader = result.program;
  shaderType = COMPUTE_SHADER;
  viewLocation = glGetUniformLocation(shader, "view");
  iTimeLocation = glGetUniformLocation(shader, "iTime");
  return result;
}

void Renderer::DeleteShader() {
  state = State::IDLE;
  glDeleteProgram(shader);
}

void Renderer::InitRenderTexture(GLenum internalFormat) {
  glGenTextures(1, &renderTexture);
  glBindTexture(GL_TEXTURE_2D, renderTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::RenderWithComputeShader(float iTime) {
  glUseProgram(shader);
  glUniform1f(iTimeLocation, iTime);
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.view));
  glBindImageTexture(0, renderTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                     GL_RGBA32F);
  glDispatchCompute(workgroupCountX, workgroupCountY, 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
