#pragma once

#include <FileWatch/FileWatch.hpp>
#include <string>

#include "gl.h"

class Reloader {
 public:
  bool needReload = true;

  /// Set the watching file and start watching
  void SetWatchFile(const std::string& newPath);

  /// Stop watching the currently set file for change
  void Stop();

 private:
  std::string watchPath;
  std::unique_ptr<filewatch::FileWatch<std::string>> watcher;
  float lastChanged = 0;

  void StartWatch(std::string& path);
};
