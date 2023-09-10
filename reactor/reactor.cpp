#include "reactor.h"

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstdlib>
#include <cstring>
#include <string>

#include "common.h"
#include "epoll_selector.h"
#include "event.h"
#include "tcp_conn.h"

namespace reactor {
Reactor::Reactor(std::string ip, int port) : tcp_conn_(ip, port), ep_selector_() { events_.resize(max_epoll_events_); }

void Reactor::add_listener() {
  // 初始化ntyevent事件结构, 然后添加到reactor的epoll事件表中即可
  log("call add_listener");
  int fd = tcp_conn_.sock_fd;
  log(std::to_string(fd) + " " + std::to_string(events_.size()));
  events_[fd] =
      Event{.fd_ = fd, .events_ = EPOLLIN, .arg_ = this, .callback_ = acb_, .status_ = 0, .length_ = 0, .last_active_ = time(NULL)};
  ep_selector_.register_event(events_[fd]);
}

void Reactor::run() {
  // 1.判断参数
  if (ep_selector_.epfd_ < 0 || events_.size() == 0) {
    log("reactor arg is error");
    exit(1);
  }

  std::vector<epoll_event> ep_events(max_epoll_events_ + 1);

  // 2.进行epoll_wait()
  int nready;
  while (1) {
    nready = epoll_wait(ep_selector_.epfd_, ep_events.data(), max_epoll_events_, 1000);
    // 3.函数出错
    if (nready == -1) {
      // 如果函数在阻塞过程中接收到信号, 那么继续进行epoll_wait()
      if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
      log("Error in %s(), epoll_wait: %s\n");
    }
    // 4.函数超时
    else if (nready == 0)
      continue;
    // 5.有事件准备好
    else {
      // 遍历处理已就绪的事件
      int i;
      for (i = 0; i < nready; ++i) {
        // 获取事件结构体, 保存在struct epoll_event结构的data.ptr中
        Event *ev = (Event *)ep_events[i].data.ptr;

        // if (ev->fd_ == tcp_conn_.sock_fd) {
        //   acb_(ev->fd_, ev->events_, ev->arg_);
        //   continue;
        // }

        // 如果事件可读
        if ((ep_events[i].events & EPOLLIN) && (ev->events_ & EPOLLIN)) {
          ev->callback_(ev->fd_, ev->events_, ev->arg_);
        }

        // 如果事件可写
        if ((ep_events[i].events & EPOLLOUT) && (ev->events_ & EPOLLOUT)) {
          ev->callback_(ev->fd_, ev->events_, ev->arg_);
        }
      }
    }
  }
}
}  // namespace reactor