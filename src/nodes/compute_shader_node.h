#include <memory>
#include <string>

#include "gl.h"
#include "node.h"
#include "render_context.h"

namespace node {

class ComputeShaderNode : public Node {
 public:
  void Init() override;

  void Execute(renderer::Context& ctx) override;

  void SetSize(int width, int height) override;

  void SetProgramPath(const std::string& path) override;

  void Validate() override;

 private:
  int workgroupCountX = 1;
  int workgroupCountY = 1;

  GLuint colorbuffer;
  std::shared_ptr<GLuint> shader;
  std::string shaderPath;

  GLuint viewLocation;
  GLuint iTimeLocation;

  void CompileShader();
};

}  // namespace node
