#include "Config.h"

int main(int argc, char* argv[]) {
  const int width = argc > 1 ? atoi(argv[1]) : 640;
  const int height = argc > 2 ? atoi(argv[2]) : 480;
  const char* title = argc > 3 ? argv[3] : "Shader Renderer";

  UI ui = UI();
  if (!ui.InitUI(width, height, title)) return 1;

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
