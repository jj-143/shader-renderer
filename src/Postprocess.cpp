#include "Postprocess.h"

Postprocess::Postprocess(const int& width, const int& height,
                         const char* filepathVertex,
                         const char* filepathFragment)
    : width(width), height(height) {
  shader = MakeShader(filepathVertex, filepathFragment);
}

void Postprocess::Update() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT);

  // Screen
  glUseProgram(shader);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, colorbuffer);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLuint Postprocess::CreateFramebuffer() {
  colorbuffer = CreateColorbuffer(width, height, GL_RGBA8);
  depthbuffer = CreateDepthbuffer(width, height);

  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         colorbuffer, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, depthbuffer);
  return framebuffer;
}

GLuint Postprocess::CreateColorbuffer(const int& width, const int& height,
                                      GLenum internalFormat) {
  GLuint colorbuffer;
  glGenTextures(1, &colorbuffer);
  glBindTexture(GL_TEXTURE_2D, colorbuffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
  return colorbuffer;
}

GLuint Postprocess::CreateDepthbuffer(const int& width, const int& height) {
  GLuint depthbuffer;
  glGenRenderbuffers(1, &depthbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  return depthbuffer;
}