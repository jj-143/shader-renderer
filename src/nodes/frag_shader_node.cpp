#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "gl.h"
#include "logger.h"
#include "node_registry.h"
#include "render_context.h"
#include "shader.h"
#include "shader_manager.h"
#include "shader_node.h"

namespace {

using namespace node;

const auto Spec = NodeInfo{
    .name = "FragShaderNode",
    .label = "",
    .inputs = {Input(InputType::File, "Path")},
    .uniforms = {},
};

}  // namespace

namespace node {

class FragShaderNode : public ShaderNode {
 public:
  FragShaderNode() : ShaderNode("FragShaderNode") {}

  std::shared_ptr<ShaderNode> Clone() override {
    return std::make_shared<FragShaderNode>(*this);
  }

  const NodeInfo& GetSpec() const override { return Spec; }

  void Init() override {
    // Colorbuffer
    glGenTextures(1, &colorbuffer);
    glBindTexture(GL_TEXTURE_2D, colorbuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // FBO
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorbuffer, 0);

    output = colorbuffer;
  }

  void Execute(renderer::Context& ctx) override {
    if (!isValid) return;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader->program);
    glBindImageTexture(0, colorbuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                       GL_RGBA32F);

    glUniform1f(iTimeLocation, ctx.iTime);

    // "tDiffuse"
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, input);

    glUniform1i(tDiffuseLocation, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  void SetSize(int width, int height) override {
    this->width = width;
    this->height = height;
    glBindTexture(GL_TEXTURE_2D, colorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
                 GL_FLOAT, NULL);
  }

  void Validate(renderer::Context& ctx) override {
    auto& shaderPath = GetInput("Path")->Value<std::string>();

    shader = ctx.shaderManager.GetShader(shaderPath, ShaderType::FragShader);

    if (!shader) {
      isValid = false;
      return;
    }

    isValid = true;

    iTimeLocation = glGetUniformLocation(shader->program, "iTime");
    tDiffuseLocation = glGetUniformLocation(shader->program, "tDiffuse");
  }

 private:
  GLuint framebuffer;
  GLuint colorbuffer;
  std::shared_ptr<Shader> shader;

  GLuint iTimeLocation;
  GLuint tDiffuseLocation;
};

}  // namespace node

namespace node::registry {

void RegisterFragShaderNode() {
  NodeEntry entry{
      .spec = Spec,
      .Create = [] { return std::make_shared<FragShaderNode>(); },
  };
  RegisterNode(entry);
}

}  // namespace node::registry
