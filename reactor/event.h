#pragma once

#include <sys/epoll.h>

#include <functional>

#define MAX_BUFFER_SIZE 4096

namespace reactor {
struct Event {
  int fd_;
  int events_;  // 事件类型（  本代码中我们只处理EPOLL_IN和EPOLL_OUT）
  void *arg_;   // 事件回调函数的参数3, 实际传入的是一个struct ntyreactor结构体指针
  // void (*callback_)(int fd, int events, void *arg);
  std::function<void(int, int, void *)> callback_;  // 事件回调函数
  int status_;

  char buffer_[MAX_BUFFER_SIZE];  // 读写缓冲区
  int length_;                    //缓冲区数据的长度

  long last_active_;  // 最后一次活跃的时间
};
}  // namespace reactor