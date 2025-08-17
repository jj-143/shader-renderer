#include "reloader.h"

#include <filesystem>

#include "ops.h"

void Reloader::SetWatchFile(const std::string& newPath) {
  std::filesystem::path p{newPath};
  p = std::filesystem::absolute(p);
  p = p.lexically_normal();

  std::string newPathCanonical = p.string();
  if (watchPath == newPathCanonical) return;
  watchPath = newPathCanonical;
  StartWatch();
}

void Reloader::Stop() {
  watchPath.clear();
  watcher.reset();
}

/**
 * Debounce needed for modified event firing 2 times.
 * see: https://github.com/ThomasMonkman/filewatch/issues/27
 * They said it's Windows API related, but it happens on Linux too.
 */
void Reloader::StartWatch() {
  watcher = std::make_unique<filewatch::FileWatch<std::string>>(
      watchPath, [this]([[maybe_unused]] const std::string& path,
                        const filewatch::Event changeType) {
        if (changeType == filewatch::Event::modified) {
          float now = glfwGetTime();
          if (now - lastChanged < 0.1) return;
          lastChanged = now;
          needReload = true;
        }
      });
}
