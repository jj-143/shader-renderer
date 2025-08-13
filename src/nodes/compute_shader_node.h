#include <memory>
#include <string>

#include "gl.h"
#include "node.h"
#include "render_context.h"
#include "shader.h"

namespace node {

class ComputeShaderNode : public Node {
 public:
  std::shared_ptr<Node> Clone() override;

  void Init() override;

  void Execute(renderer::Context& ctx) override;

  void SetSize(int width, int height) override;

  void SetProgramPath(const std::string& path) override;

  void Validate(renderer::Context& ctx) override;

 private:
  int workgroupCountX = 1;
  int workgroupCountY = 1;

  GLuint colorbuffer;
  std::shared_ptr<Shader> shader;
  std::string shaderPath;

  GLuint viewLocation;
  GLuint iTimeLocation;
};

}  // namespace node
