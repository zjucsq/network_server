#pragma once

#include <atomic>
#include <coroutine>
#include <utility>

#include "executor.h"
#include "scheduler.h"

namespace asyncio {
template <typename T, typename E>
struct task_promise;

template <typename T, typename E>
struct FinalAwaiter {
  FinalAwaiter() noexcept {}

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<task_promise<T, E>> handle) const noexcept { handle.promise().set_finish(); }

  void await_resume() noexcept {}
};
}  // namespace asyncio
