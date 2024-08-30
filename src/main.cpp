#include <cstdlib>
#include <string>

#include "Global.h"
#include "app.h"

int main(int argc, char* argv[]) {
  const int width = argc > 1 ? atoi(argv[1]) : Global::DEFAULT_VIEWPORT_WIDTH;
  const int height = argc > 2 ? atoi(argv[2]) : Global::DEFAULT_VIEWPORT_HEIGHT;
  const char* title = argc > 3 ? argv[3] : Global::DEFAULT_WINDOW_TITLE;
  const std::string shaderPath = argc > 4 ? argv[4] : Global::DEFAULT_SHADER;

  const App::Config config = {
      .vW = width, .vH = height, .title = title, .shaderPath = shaderPath};

  App app = App(config);

  return app.Run();
}
