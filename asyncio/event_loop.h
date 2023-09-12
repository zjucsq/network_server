#pragma once

#include <chrono>

#include "net/epoll_selector.h"
#include "noncopyable.h"

namespace asyncio {
class EventLoop : NonCopyable {
  using MSDuration = std::chrono::milliseconds;

 public:
  EventLoop();

  void run();

 private:
  EpollSelector ep_selector_;
};
}  // namespace asyncio