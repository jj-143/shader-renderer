#pragma once

#include <atomic>
#include <string>
#include <thread>

class Reloader {
 public:
  ~Reloader();

  /// Reload active shader if it changed.
  void HandleReload();

  void Start();

  void Stop();

  /// Set active shader to watch for change.
  bool WatchForChange(std::string path);

 private:
  int wd;
  std::atomic_bool isCanceled = false;
  std::atomic_bool needReload = false;
  std::string activeFile;
  std::unique_ptr<std::thread> watcherThread;
};
