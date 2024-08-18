#pragma once
#include <glad/gl.h>

class Postprocess {
 public:
  GLuint framebuffer;
  GLuint colorbuffer;
  GLuint depthbuffer;
  GLuint shader;
  const int& width;
  const int& height;

  Postprocess(const int& width, const int& height, const char* filepathVertex,
              const char* filepathFragment);
  GLuint CreateFramebuffer();
  void Update();

  static GLuint CreateColorbuffer(const int& width, const int& height,
                                  GLenum internalFormat);
  static GLuint CreateDepthbuffer(const int& width, const int& height);
};