#include <cstdlib>
#include <string>

#include "app.h"
#include "global.h"
#include "logger.h"

int main(int argc, char* argv[]) {
  const int width = argc > 1 ? atoi(argv[1]) : global::DEFAULT_VIEWPORT_WIDTH;
  const int height = argc > 2 ? atoi(argv[2]) : global::DEFAULT_VIEWPORT_HEIGHT;
  const std::string path = argc > 3 ? argv[3] : "";
  const char* title = argc > 4 ? argv[4] : global::DEFAULT_WINDOW_TITLE;

  const app::Args args{
      .vW = width,
      .vH = height,
      .title = title,
      .path = path,
  };

  app::App app;

  if (!app.Init(args)) {
    logger::Error("Cannot start the App. Exiting..");
    return 1;
  }

  app.Run();
}
