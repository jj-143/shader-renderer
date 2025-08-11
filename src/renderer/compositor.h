#pragma once

#include <memory>

#include "gl.h"
#include "node.h"
#include "render_context.h"

namespace renderer {

class Compositor {
 public:
  bool isValid = false;
  std::string errorLog;

  std::shared_ptr<node::Node> node;

  GLuint output;

  void Init();

  void Execute(Context &ctx);

  void Validate();

  void SetSize(int width, int height);

 private:
  int width;
  int height;
};

}  // namespace renderer
