#include "file_dialog.h"

#include <nfd/nfd.h>

#include <format>

#include "ops.h"

namespace file_dialog {

std::optional<std::string> OpenFile(fs::path path) {
  nfdchar_t* outChar;
  nfdresult_t result =
      NFD_OpenDialog(nullptr, (nfdchar_t*)path.c_str(), &outChar);

  std::optional<std::string> selected;

  switch (result) {
    case NFD_ERROR:
      ops::ReportError("Cannot load file ({})", NFD_GetError());
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

}  // namespace file_dialog