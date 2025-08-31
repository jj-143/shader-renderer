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

class Compositor : public std::enable_shared_from_this<Compositor> {
 public:
  bool isValid = true;
  bool needValidation = false;
  bool valueChanged = false;

  std::vector<error::Error> errors;

  GLuint output;

  static std::shared_ptr<Compositor> Create();

  std::vector<std::shared_ptr<node::ShaderNode>> GetNodes();
  std::shared_ptr<Compositor> GetShared();

  std::shared_ptr<Compositor> Clone();

  void Init();

  void Execute(Context &ctx);

  void Validate(Context &ctx, std::vector<error::Error> &errors);

  void SetSize(int width, int height);

 private:
  int width;
  int height;

  Compositor() = default;

  std::vector<std::shared_ptr<node::ShaderNode>> nodes;

  friend std::shared_ptr<Compositor> BuildCompositor(project::ProjectInfo);
  friend bool AddNode(const std::string &name, Compositor &compositor);
  friend bool RemoveNode(node::Node &target, Compositor &compositor);
};

std::shared_ptr<Compositor> BuildCompositor(project::ProjectInfo);

bool AddNode(const std::string &name, Compositor &compositor);

bool RemoveNode(node::Node &target, Compositor &compositor);

}  // namespace renderer
