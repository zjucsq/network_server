#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "event.h"

namespace reactor {
class EpollSelector {
  friend class Reactor;

 public:
  EpollSelector();
  ~EpollSelector();

  std::vector<epoll_event> select(int timeout);

  void register_event(Event& event);
  void remove_event(Event& event);

 private:
  int epfd_;
  int register_event_count_;
};
}  // namespace reactor