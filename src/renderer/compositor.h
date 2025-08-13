#pragma once

#include <memory>
#include <vector>

#include "gl.h"
#include "node.h"
#include "render_context.h"

namespace renderer {

class Compositor {
 public:
  bool isValid = false;
  std::string errorLog;

  GLuint output;

  std::vector<std::shared_ptr<node::Node>> GetNodes();

  void Init();

  void Execute(Context &ctx);

  void Validate();

  void SetSize(int width, int height);

 private:
  int width;
  int height;

  std::vector<std::shared_ptr<node::Node>> nodes;

  void LinkNodes();
};

}  // namespace renderer
