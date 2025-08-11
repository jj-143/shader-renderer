#pragma once

#include <string>

namespace renderer {
struct Context;
}

namespace node {

class Node {
 public:
  GLuint input;
  GLuint output;

  bool isValid = false;
  std::string errorLog = "";

  virtual void Init() {}

  virtual void Execute(renderer::Context& ctx) {}

  virtual void SetSize(int width, int height) {}

  virtual void SetProgramPath(const std::string& path) {}

 protected:
  int width;
  int height;
};

}  // namespace node