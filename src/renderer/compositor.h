#pragma once

#include <memory>
#include <vector>

#include "error.h"
#include "gl.h"
#include "render_context.h"
#include "shader_node.h"

namespace project {
struct ProjectInfo;
}

namespace renderer {

class Compositor {
 public:
  bool isValid = true;
  bool needValidation = false;

  std::vector<error::Error> errors;

  GLuint output;

  std::vector<std::shared_ptr<node::ShaderNode>> GetNodes();

  Compositor Clone();

  void Init();

  void Execute(Context &ctx);

  void Validate(Context &ctx, std::vector<error::Error> &errors);

  void SetSize(int width, int height);

 private:
  int width;
  int height;

  std::vector<std::shared_ptr<node::ShaderNode>> nodes;

  friend Compositor BuildCompositor(project::ProjectInfo);
  friend bool AddNode(const std::string &name, Compositor &compositor);
};

Compositor BuildCompositor(project::ProjectInfo);

bool AddNode(const std::string &name, Compositor &compositor);

}  // namespace renderer
