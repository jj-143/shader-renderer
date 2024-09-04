#pragma once
#include "../common.h"
#include "Camera.h"

class Renderer {
 public:
  int w;
  int h;
  GLuint colorbuffer;
  Camera camera;

  GLuint viewLocation;
  GLuint iTimeLocation;

  void Init(int width, int height, GLenum colorbufferFormat = GL_RGBA32F);
  void SetComputeShader(const char* path);
  void DeleteShader();
  void Render(float iTime);

 private:
  enum ShaderType { COMPUTE_SHADER };

  bool initialized;
  GLuint shader;
  ShaderType shaderType = COMPUTE_SHADER;

  // COMPUTE_SHADER Only
  int workgroupCountX = 1;
  int workgroupCountY = 1;

  void RenderWithComputeShader(float iTime);
};
