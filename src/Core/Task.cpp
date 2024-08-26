#include "Task.h"

#include <cassert>

namespace Task {
Task::Task(const char *name, const Function &f) : name(name), fn(f) {}

Task::~Task() { thread.reset(); }

void Task::Cancel() { isCanceled = true; }

void Task::Run() {
  thread = std::make_unique<std::thread>(&Task::_Run, this);
  thread->detach();
}

void Task::OnCleanup(const Callback &f) { this->cleanup = f; };
void Task::_Run() {
  fn(*this);
  if (cleanup) cleanup();
}
}  // namespace Task

namespace Task {
void TaskManager::RunTask(std::unique_ptr<Task> _task) {
  assert(!HasTask() && "There's already a running Task.");
  task = std::move(_task);
  task->OnCleanup([&]() { task.reset(); });
  task->Run();
}

void TaskManager::CancelTask() { task->Cancel(); }
}  // namespace Task
