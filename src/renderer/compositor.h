#pragma once

#include <memory>
#include <vector>

#include "error.h"
#include "gl.h"
#include "node.h"
#include "render_context.h"

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

  std::vector<std::shared_ptr<node::Node>> GetNodes();

  Compositor Clone();

  void Init();

  void Execute(Context &ctx);

  void Validate(Context &ctx, std::vector<error::Error> &errors);

  void SetSize(int width, int height);

 private:
  int width;
  int height;

  std::vector<std::shared_ptr<node::Node>> nodes;

  void LinkNodes();

  friend Compositor BuildCompositor(project::ProjectInfo);
};

Compositor BuildCompositor(project::ProjectInfo);

}  // namespace renderer
