#pragma once
#include "../common.h"
#include "Camera.h"
#include "ShaderLoader.h"

class Renderer {
 public:
  GLuint renderTexture;
  Camera camera;

  GLuint viewLocation;
  GLuint iTimeLocation;

  void Init(int width, int height, GLenum colorbufferFormat = GL_RGBA32F);
  ShaderCompileResult SetComputeShader(const char* path);
  void DeleteShader();
  void Render(float iTime);
  void SetSize(int width, int height);

 private:
  GLenum format;
  enum ShaderType { COMPUTE_SHADER };

  bool initialized;
  GLuint shader;
  ShaderType shaderType = COMPUTE_SHADER;

  int workgroupCountX = 1;
  int workgroupCountY = 1;

  void InitRenderTexture(GLenum internalFormat);
  void RenderWithComputeShader(float iTime);
};
