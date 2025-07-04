#pragma once

#include <atomic>
#include <functional>
#include <thread>

namespace Task {

class Task {
  using Function = std::function<void(Task &)>;
  using Callback = std::function<void(void)>;
  friend class TaskManager;

 public:
  std::atomic_bool isCanceled = false;
  const char *name;
  float progress = 0;
  Function fn;
  Callback cleanup;

  Task(const char *name, const Function &f);
  ~Task();

  void Cancel();
  void Run();

 private:
  std::unique_ptr<std::thread> thread;

  void OnCleanup(const Callback &f);
  void _Run();
};

class TaskManager {
 public:
  std::unique_ptr<Task> task;

  inline bool HasTask() const { return this->task != nullptr; }
  void RunTask(std::unique_ptr<Task> _task);
  void CancelTask();
};

template <typename F, typename... Args>
std::unique_ptr<Task> CreateTask(const char *name, F const f, Args... args) {
  return std::make_unique<Task>(name,
                                std::bind(f, std::placeholders::_1, args...));
}
}  // namespace Task
