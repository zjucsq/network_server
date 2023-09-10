#pragma once

#include <sys/epoll.h>
namespace reactor {
class EpollSelector {
 public:
  EpollSelector() : epfd_(epoll_create1(0)), register_event_count_(0) {
    if (epfd_ < 0) {
    }
  }

 private:
  int epfd_;
  int register_event_count_;
};
}  // namespace reactor