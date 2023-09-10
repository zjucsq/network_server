#include "epoll_selector.h"

#include <sys/epoll.h>

#include <cstring>
#include <iostream>
#include <string_view>
#include <vector>

#include "common.h"
#include "event.h"

namespace reactor {
EpollSelector::EpollSelector() : epfd_(epoll_create1(0)), register_event_count_(0) {
  if (epfd_ < 0) {
    std::cerr << "epoll_create1 error" << std::endl;
    exit(1);
  }
}
EpollSelector::~EpollSelector() {
  if (epfd_ > 0) {
    close(epfd_);
  }
}

std::vector<epoll_event> EpollSelector::select(int timeout) {
  std::vector<epoll_event> events;
  return events;
}

void EpollSelector::register_event(Event& event) {
  // 1.创建一个epoll事件结构
  epoll_event ep_event;
  memset(&ep_event, 0, sizeof(ep_event));
  ep_event.events = event.events_;
  ep_event.data.ptr = const_cast<Event*>(&event);
  // ep_event.data.fd = ev->fd; data成员是一个联合体, 不能同时使用fd和ptr成员

  // 2.如果当前ev已经在epoll事件表中, 那么就修改; 否则就把ev加入到epoll事件表中
  int op;
  if (event.status_ == 0) {
    op = EPOLL_CTL_ADD;
    event.status_ = 1;
  } else
    op = EPOLL_CTL_MOD;

  // 3.添加/更新
  if (epoll_ctl(epfd_, op, event.fd_, &ep_event) == -1) {
    log(std::string("Error in %s(), epoll_ctl: ") + strerror(errno));
  }
}

void EpollSelector::remove_event(Event& event) {
  epoll_event ep_event;
  memset(&ep_event, 0, sizeof(ep_event));
  ep_event.data.ptr = const_cast<Event*>(&event);
  int op = EPOLL_CTL_DEL;

  if (epoll_ctl(epfd_, op, event.fd_, &ep_event) == -1) {
    log(std::string("Error in %s(), epoll_ctl: ") + strerror(errno));
  }
}

}  // namespace reactor