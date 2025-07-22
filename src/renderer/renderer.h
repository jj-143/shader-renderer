#pragma once

#include "camera.h"
#include "common.h"
#include "shader_loader.h"

class Renderer {
 public:
  GLuint renderTexture;
  Camera camera;

  GLuint viewLocation;
  GLuint iTimeLocation;

  std::string errorLog;

  inline bool IsIdle() { return state == State::IDLE; }
  inline bool IsCompileSuccess() { return state == State::RUNNING; }
  inline bool IsCompileError() { return state == State::COMPILE_ERROR; }

  void Init(int width, int height, GLenum colorbufferFormat = GL_RGBA32F);
  ShaderCompileResult SetComputeShader(const char* path);
  void DeleteShader();
  void Render(float iTime);
  void SetSize(int width, int height);

 private:
  GLenum format;
  enum ShaderType { COMPUTE_SHADER };
  enum class State { IDLE, RUNNING, COMPILE_ERROR };

  bool initialized;
  State state = State::IDLE;
  GLuint shader;
  ShaderType shaderType = COMPUTE_SHADER;

  int workgroupCountX = 1;
  int workgroupCountY = 1;

  void InitRenderTexture(GLenum internalFormat);
  void RenderWithComputeShader(float iTime);
};
