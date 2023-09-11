#pragma once

#include <sys/epoll.h>

#include <cerrno>
#include <iostream>
#include <vector>

#include "event.h"

class EpollSelector {
 public:
  EpollSelector() : epfd_(epoll_create1(0)) {
    if (epfd_ < 0) {
      std::cerr << "epoll_create1 failed" << std::endl;
    }
  }
  std::vector<Event> select(int timeout) {
    errno = 0;
    std::vector<epoll_event> events;
    events.resize(register_event_count_);
    int ndfs = epoll_wait(epfd_, events.data(), register_event_count_, timeout);
    std::vector<Event> results;
    for (int i = 0; i < ndfs; ++i) {
      auto handle_info = reinterpret_cast<HandleInfo*>(events[i].data.ptr);
      if (handle_info->handle != nullptr && handle_info->handle != (Handle*)&handle_info->handle) {
        results.emplace_back(Event{.handle_info = *handle_info});
      } else {
        handle_info->handle = (Handle*)&handle_info->handle;
      }
    }
    return results;
  }
  ~EpollSelector() {
    if (epfd_ > 0) {
      close(epfd_);
    }
  }
  bool is_stop() { return register_event_count_ == 1; }
  void register_task(const Event& event) {
    epoll_event ev{.events = event.flags, .data{.ptr = const_cast<HandleInfo*>(&event.handle_info)}};
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, event.fd, &ev) == 0) {
      ++register_event_count_;
    }
  }
  void remove_task(const Event& event) {
    epoll_event ev{.events = event.flags};
    if (epoll_ctl(epfd_, EPOLL_CTL_DEL, event.fd, &ev) == 0) {
      --register_event_count_;
    }
  }

 private:
  int epfd_;
  int register_event_count_{1};
};