#include <memory>
#include <string>

#include "gl.h"
#include "render_context.h"
#include "shader.h"
#include "shader_node.h"

namespace node {

class ComputeShaderNode : public ShaderNode {
 public:
  ComputeShaderNode() : ShaderNode("ComputeShaderNode") {}

  static const NodeInfo Spec;

  const NodeInfo& GetSpec() const override;

  std::shared_ptr<ShaderNode> Clone() override;

  void Init() override;

  void Execute(renderer::Context& ctx) override;

  void SetSize(int width, int height) override;

  void Validate(renderer::Context& ctx) override;

 private:
  int workgroupCountX = 1;
  int workgroupCountY = 1;

  GLuint colorbuffer;
  std::shared_ptr<Shader> shader;

  GLuint viewLocation;
  GLuint iTimeLocation;
};

}  // namespace node
