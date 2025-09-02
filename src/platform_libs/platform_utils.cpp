#include "platform_utils.h"

#include <limits.h>

#if defined(__linux__)

#include <unistd.h>

#elif defined(__APPLE__)

#include <mach-o/dyld.h>

#include <climits>

#elif defined(_WIN32)

#include <windows.h>

#endif

#if defined(__linux__)

namespace platform_utils {

std::filesystem::path GetBinaryRoot() {
  char szPath[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", szPath, PATH_MAX);
  if (count < 0 || count >= PATH_MAX) return {};  // some error
  szPath[count] = '\0';
  return std::filesystem::path{szPath}.parent_path() / "";  // trailing slash
}

}  // namespace platform_utils

#elif defined(__APPLE__)

namespace platform_utils {

std::filesystem::path GetBinaryRoot() {
  char szPath[PATH_MAX];
  uint32_t bufsize = PATH_MAX;
  if (!_NSGetExecutablePath(szPath, &bufsize))
    return std::filesystem::path{szPath}.parent_path() / "";  // trailing slash
  return {};                                                  // some error
}

}  // namespace platform_utils

#elif defined(_WIN32)

namespace platform_utils {

std::filesystem::path GetBinaryRoot() {
  wchar_t szPath[MAX_PATH];
  GetModuleFileNameW(NULL, szPath, MAX_PATH);
  return std::filesystem::path{szPath}.parent_path() / "";  // trailing slash
}

}  // namespace platform_utils

#else

#error "Unsupported platform"

#endif
