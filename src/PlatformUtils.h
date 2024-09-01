#pragma once

#include <limits.h>

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>

#include <climits>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

namespace PlatformUtils {
inline fs::path GetBinaryRoot() {
#ifdef _WIN32
  // Windows specific
  wchar_t szPath[MAX_PATH];
  GetModuleFileNameW(NULL, szPath, MAX_PATH);
#elif __APPLE__
  char szPath[PATH_MAX];
  uint32_t bufsize = PATH_MAX;
  if (!_NSGetExecutablePath(szPath, &bufsize))
    return std::filesystem::path{szPath}.parent_path() / "";  // trailing slash
  return {};                                                  // some error
#else
  // Linux specific
  char szPath[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", szPath, PATH_MAX);
  if (count < 0 || count >= PATH_MAX) return {};  // some error
  szPath[count] = '\0';
#endif
  return std::filesystem::path{szPath}.parent_path() / "";  // railing slash
}
}  // namespace PlatformUtils
