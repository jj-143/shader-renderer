#include "app.h"

#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Postprocess.h"
#include "ShaderLoader.h"

App::App(const Config config) : config(config) {
  assert(instance == nullptr && "App already instantiated.");
  instance = this;
};

int App::Run() {
  if (!ui.InitUI(config.vW, config.vH, config.title)) {
    return 1;
  }

  GLuint colorbuffer = Postprocess::CreateColorbuffer(ui.vW, ui.vH, GL_RGBA32F);
  ui.viewportTextureID = colorbuffer;

  GLuint computeShader = MakeComputeShader("../res/shaders/raytracer.comp");
  GLuint viewLocation = glGetUniformLocation(computeShader, "view");
  GLuint iTimeLocation = glGetUniformLocation(computeShader, "iTime");
  int workgroupCountX = (ui.vW + 7) / 8;
  int workgroupCountY = (ui.vH + 7) / 8;

  Camera camera = Camera();
  camera.rotation = {0, 0, 90};  // forward: +Y, right: +X, up: +Z
  camera.Update();
  ui.SetCamera(camera);

  while (ui.NewFrame()) {
    glUseProgram(computeShader);
    glUniform1f(iTimeLocation, ui.iTime);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.view));
    glBindImageTexture(0, colorbuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                       GL_RGBA32F);
    glDispatchCompute(workgroupCountX, workgroupCountY, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    ui.Render();
  }

  UI::Terminate();
  return 0;
}
