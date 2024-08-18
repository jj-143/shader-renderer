#include <cstdlib>

#include "app.h"

int main(int argc, char* argv[]) {
  const int width = argc > 1 ? atoi(argv[1]) : 640;
  const int height = argc > 2 ? atoi(argv[2]) : 480;
  const char* title = argc > 3 ? argv[3] : "Shader Renderer";

  App app = App({width, height, title});

  return app.Run();
}
