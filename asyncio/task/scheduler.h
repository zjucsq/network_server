#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

#include "io_utils.h"

namespace asyncio {
class DelayedExecutable {
 public:
  DelayedExecutable(std::function<void()> &&func, long long delay) : func_(std::move(func)) {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto current = duration_cast<milliseconds>(now.time_since_epoch()).count();
    scheduled_time_ = current + delay;
  }

  long long delay() const {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto current = duration_cast<milliseconds>(now.time_since_epoch()).count();
    return scheduled_time_ - current;
  }
  long long get_scheduled_time() const { return scheduled_time_; }

  void operator()() { func_(); }

  bool operator<(const DelayedExecutable &rhs) const { return scheduled_time_ > rhs.get_scheduled_time(); }

 private:
  long long scheduled_time_;
  std::function<void()> func_;
};

class Scheduler {
  std::condition_variable queue_condition;
  std::mutex queue_lock;
  std::priority_queue<DelayedExecutable> executable_queue;

  std::atomic_bool is_active;
  std::thread work_thread;

  bool is_queue_empty() {
    std::lock_guard lock(queue_lock);
    return executable_queue.empty();
  }

  // TODO：条件变量是否应该使用while防止虚假唤醒
  void run_loop() {
    // while (is_active.load(std::memory_order_relaxed) || !is_queue_empty()) {
    //   std::unique_lock lock(queue_lock);
    //   if (executable_queue.empty()) {
    //     queue_condition.wait(lock);
    //     if (executable_queue.empty()) {
    //       continue;
    //     }
    //   }
    while (true) {
      std::unique_lock lock(queue_lock);
      queue_condition.wait(lock, [this]() { return !is_active.load(std::memory_order_seq_cst) || !executable_queue.empty(); });
      if (executable_queue.empty()) {
        if (is_active.load(std::memory_order_seq_cst)) {
          continue;
        } else {
          break;
        }
      }

      auto executable = executable_queue.top();
      long long delay = executable.delay();
      if (delay > 0) {
        auto status = queue_condition.wait_for(lock, std::chrono::milliseconds(delay));
        if (status != std::cv_status::timeout) {
          continue;
        }
      }
      executable_queue.pop();
      lock.unlock();
      executable();
    }
    debug("scheduler exit.");
  }

 public:
  Scheduler() {
    is_active.store(true, std::memory_order_seq_cst);
    work_thread = std::thread(&Scheduler::run_loop, this);
  }

  ~Scheduler() {
    debug("destroy scheduler start.");
    shutdown(false);
    join();
    debug("destroy scheduler finish.");
  }

  // TODO：notify之前是否应该持有锁
  void execute(std::function<void()> &&func, long long delay) {
    delay = std::max(0ll, delay);
    std::unique_lock lock(queue_lock);
    if (is_active.load(std::memory_order_seq_cst)) {
      bool need_notify = executable_queue.empty() || executable_queue.top().delay() > delay;
      executable_queue.push(DelayedExecutable{std::move(func), delay});
      lock.unlock();
      if (need_notify) {
        queue_condition.notify_one();
      }
    }
  }

  void shutdown(bool wait_for_complete = true) {
    is_active.store(false, std::memory_order_seq_cst);
    if (!wait_for_complete) {
      std::lock_guard lock(queue_lock);
      decltype(executable_queue) empty_queue;
      std::swap(executable_queue, empty_queue);
    }
    queue_condition.notify_all();
  }

  void join() {
    if (work_thread.joinable()) {
      work_thread.join();
    }
  }
};
}  // namespace asyncio