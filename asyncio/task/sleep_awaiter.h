#pragma once

#include <coroutine>
#include <utility>

#include "executor.h"
#include "scheduler.h"

namespace asyncio {
struct SleepAwaiter {
  explicit SleepAwaiter(AbstractExecutor *executor, long long duration) noexcept : executor_(executor), duration_(duration) {}

  bool await_ready() const { return false; }

  void await_suspend(std::coroutine_handle<> handle) const {
    static Scheduler scheduler;
    scheduler.execute([this, handle]() { executor_->execute([handle]() { handle.resume(); }); }, duration_);
  }

  void await_resume() {}

 private:
  AbstractExecutor *executor_;
  long long duration_;
};
}  // namespace asyncio
