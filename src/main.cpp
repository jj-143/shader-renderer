#include <cstdlib>

#include "app.h"

int main(int argc, char* argv[]) {
  const int width = argc > 1 ? atoi(argv[1]) : 640;
  const int height = argc > 2 ? atoi(argv[2]) : 480;
  const char* title = argc > 3 ? argv[3] : "Shader Renderer";
  const char* shaderPath = argc > 4 ? argv[4] : "../res/shaders/raytracer.comp";

  const App::Config config = {
      .vW = width, .vH = height, .title = title, .shaderPath = shaderPath};

  App app = App(config);

  return app.Run();
}
