
#include <string>

namespace Ops {

/// Cancel Current running task in [App::taskManager]
bool CancelTask();

/// Display a message in Status Bar.
bool Report(std::string message);

/// Render to output file.
/// if `animation` is set, renders frames in the range defined in the Setting.
/// Otherwise, renders current iTime in `Timeline`.
bool Render(bool animation = false);

}  // namespace Ops
