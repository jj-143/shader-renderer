#include "compute_shader_node.h"

#include <format>
#include <glm/gtc/type_ptr.hpp>

#include "gl.h"
#include "logger.h"
#include "shader_loader.h"

namespace node {

void ComputeShaderNode::Init() {
  glGenTextures(1, &colorbuffer);
  glBindTexture(GL_TEXTURE_2D, colorbuffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  output = colorbuffer;
}

void ComputeShaderNode::Execute(renderer::Context& ctx) {
  glUseProgram(*shader);
  glBindImageTexture(0, colorbuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

  glUniform1f(iTimeLocation, ctx.iTime);
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(*ctx.view));

  glDispatchCompute(workgroupCountX, workgroupCountY, 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShaderNode::SetSize(int width, int height) {
  this->width = width;
  this->height = height;
  workgroupCountX = (width + 7) / 8;
  workgroupCountY = (height + 7) / 8;
  glBindTexture(GL_TEXTURE_2D, colorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
               GL_FLOAT, NULL);
}

void ComputeShaderNode::SetProgramPath(const std::string& path) {
  if (shader) {
    glDeleteProgram(*shader);
  }

  ShaderCompileResult result = MakeComputeShader(path);

  if (!result.isSuccess) {
    isValid = false;
    errorLog = std::format("{}\n\n{}", path, result.error);
    return;
  }

  isValid = true;
  errorLog = "";

  shader = std::make_shared<GLuint>(result.program);
  viewLocation = glGetUniformLocation(*shader, "view");
  iTimeLocation = glGetUniformLocation(*shader, "iTime");
}

}  // namespace node
