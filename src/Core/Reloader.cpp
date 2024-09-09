#include "Reloader.h"

#include <inotifytools/inotify.h>
#include <inotifytools/inotifytools.h>

#include <filesystem>

#include "Ops.h"

namespace fs = std::filesystem;

namespace {
std::string ToFullPath(std::string path);
}

Reloader::~Reloader() {
  Stop();  //
};

void Reloader::Start() {
  inotifytools_initialize();
  watcherThread = std::make_unique<std::thread>([&]() {
    inotify_event* event;
    while (true) {
      if (isCanceled) {
        return;
      }
      event = inotifytools_next_event(1);

      if (event) {
        needReload = true;
      }
    }
  });
}

void Reloader::Stop() {
  isCanceled = true;
  inotifytools_remove_watch_by_filename(activeFile.c_str());
  inotifytools_cleanup();
  watcherThread->join();
}

bool Reloader::WatchForChange(std::string filepath) {
  // printf("Watch For Change\n");
  if (activeFile != "") {
    // printf("Remove %s\n", activeFile.c_str());
    inotifytools_remove_watch_by_filename(activeFile.c_str());
  }

  try {
    activeFile = ToFullPath(filepath);
  } catch (fs::filesystem_error error) {
    Ops::Report(error.what());
    return false;
  }

  // printf("Watch %s\n", activeFile.c_str());
  wd = inotifytools_watch_file(activeFile.c_str(), IN_CLOSE_WRITE);
  if (wd == -1) {
    printf("Watch Failed for %s\n", activeFile.c_str());
    // Error such as ENOENT
    // see: https://lwn.net/Articles/605128/
    return false;
  }
  return true;
}

void Reloader::HandleReload() {
  if (needReload) {
    needReload = false;
    Ops::ReloadShader();
  }
}

namespace {
std::string ToFullPath(std::string _path) {
  fs::path path{_path};
  return fs::canonical(path).string();
}
}  // namespace
