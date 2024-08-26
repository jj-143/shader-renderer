
#include <string>

namespace Ops {

/// Cancel Current running task in [App::taskManager]
bool CancelTask();

/// Display a message in Status Bar.
bool Report(std::string message);

}  // namespace Ops
