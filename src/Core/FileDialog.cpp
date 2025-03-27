#include "FileDialog.h"

#include <nfd/nfd.h>

#include <format>

#include "Ops.h"

namespace FileDialog {
std::optional<std::string> OpenFile(fs::path path) {
  nfdchar_t* outChar;
  nfdresult_t result =
      NFD_OpenDialog(nullptr, (nfdchar_t*)path.c_str(), &outChar);

  std::optional<std::string> selected;

  switch (result) {
    case NFD_ERROR:
      Ops::ReportError(std::format("Cannot load file ({:s})", NFD_GetError()));
      break;
    case NFD_OKAY:
      selected = outChar;
      free(outChar);
      break;
    default:
      break;
  }

  return selected;
}
}  // namespace FileDialog