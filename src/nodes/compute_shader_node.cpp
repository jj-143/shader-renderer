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
    .name = "ComputeShaderNode",
    .label = "",
    .inputs = {Input(InputType::File, "Path")},
    .uniforms = {},
};

}  // namespace

namespace node {

class ComputeShaderNode : public ShaderNode {
 public:
  ComputeShaderNode() : ShaderNode("ComputeShaderNode") {}

  std::shared_ptr<ShaderNode> Clone() override {
    return std::make_shared<ComputeShaderNode>(*this);
  }

  const NodeInfo& GetSpec() const override { return Spec; }

  void Init() override {
    glGenTextures(1, &colorbuffer);
    glBindTexture(GL_TEXTURE_2D, colorbuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    output = colorbuffer;
  }

  void Execute(renderer::Context& ctx) override {
    if (!isValid) return;

    glBindImageTexture(0, colorbuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                       GL_RGBA32F);
    UseShader(ctx, shader);

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE,
                       glm::value_ptr(ctx.camera->view));

    glDispatchCompute(workgroupCountX, workgroupCountY, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }

  void SetSize(int width, int height) override {
    this->width = width;
    this->height = height;
    workgroupCountX = (width + 7) / 8;
    workgroupCountY = (height + 7) / 8;
    glBindTexture(GL_TEXTURE_2D, colorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
                 GL_FLOAT, NULL);
  }

  void Validate(renderer::Context& ctx) override {
    auto& shaderPath = GetInput("Path")->Value<std::string>();

    shader = ctx.shaderManager.GetShader(shaderPath, ShaderType::ComputeShader);

    if (!shader) {
      isValid = false;
      return;
    }

    isValid = true;

    viewLocation = glGetUniformLocation(shader->program, "view");

    RegisterShaders({shader});
  }

 private:
  int workgroupCountX = 1;
  int workgroupCountY = 1;

  GLuint colorbuffer;
  std::shared_ptr<Shader> shader;

  GLuint viewLocation;
};

}  // namespace node

namespace node::registry {

void RegisterComputeShaderNode() {
  NodeEntry entry{
      .spec = Spec,
      .Create = [] { return std::make_shared<ComputeShaderNode>(); },
  };
  RegisterNode(entry);
}

}  // namespace node::registry
