#pragma once

#include "camera.h"
#include "compositor.h"
#include "gl.h"
#include "render_context.h"

namespace renderer {

class Renderer {
 public:
  Context ctx;

  Camera camera;
  Compositor compositor;

  GLuint* renderTexture;

  std::string errorLog;

  inline bool IsIdle() { return state == State::IDLE; }
  inline bool IsCompileSuccess() { return state == State::RUNNING; }
  inline bool IsCompileError() { return state == State::COMPILE_ERROR; }

  void Init(int width, int height);
  void SetComputeShader(const char* path);
  void Render(float iTime);
  void SetSize(int width, int height);

 private:
  enum class State { IDLE, RUNNING, COMPILE_ERROR };

  bool initialized;
  State state = State::IDLE;

  void Validate();
};

}  // namespace renderer