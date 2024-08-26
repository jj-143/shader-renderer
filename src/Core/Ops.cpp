#include "Ops.h"

#include <format>
#include <string>

#include "../app.h"
#include "Task.h"

namespace Ops {
bool CancelTask() {
  Task::TaskManager& taskManager = App::GetInstance().taskManager;
  if (!taskManager.HasTask()) return false;
  taskManager.CancelTask();
  Ops::Report(std::format("Canceling [{:s}]", taskManager.task->name));
  return true;
}

bool Report(std::string message) {
  App& app = App::GetInstance();
  app.ui.UpdateStatus(message);
  return true;
}
}  // namespace Ops
