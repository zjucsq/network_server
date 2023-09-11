#include "reactor.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "event.h"

namespace reactor {

void accept_callback(int fd, int events, void *arg);
void recv_callback(int fd, int events, void *arg);
void send_callback(int fd, int events, void *arg);

void accept_callback(int fd, int events, void *arg) {
  // 1.获得reactor结构
  Reactor *reactor = static_cast<Reactor *>(arg);
  // 2.获取该fd对应的事件结构
  Event *ev = reactor->events_.data() + fd;

  // 3.初始化客户端地址结构
  struct sockaddr_in cli_addr;
  memset(&cli_addr, 0, sizeof(cli_addr));
  socklen_t len = sizeof(cli_addr);

  // 4.接收客户端
  int cli_fd;
  cli_fd = accept(ev->fd_, (struct sockaddr *)&cli_addr, &len);
  if (cli_fd == -1) {
    printf("Error in %s(), accept: %s\n", __func__, strerror(errno));
  }

  int i;
  do {
    // 5.在reactor事件表中找到第一个空位置, 用i表示新事件存放的位置, 也是其套接字的值
    // reactor->events的0、1、2、3、4都被占用了, 客户端第一个可以使用的套接字为5, 因此此处从5开始遍历
    for (i = 5; i < reactor->max_epoll_events_; ++i) {
      if (reactor->events_[i].status_ == 0) break;
    }

    // 6.如果满了, 就退出
    if (i == reactor->max_epoll_events_) {
      printf("Error in %s(): max connect limit[%d]\n", __func__, reactor->max_epoll_events_);
    }

    // 7.将套接字设置为非阻塞
    int flag = 0;
    if ((flag = fcntl(cli_fd, F_SETFL, O_NONBLOCK)) < 0) {
      printf("Error in %s(), fcntl: %s\n", __func__, strerror(errno));
    }

    // 8.将新事件添加到reactor事件表中
    // 此处我们将新客户端的回调函数首先设置为recv_callback, 事件类型为EPOLLIN, 因为一般都是客户端向服务器发送数据的
    reactor->events_[cli_fd] = Event{
        .fd_ = cli_fd, .events_ = EPOLLIN, .arg_ = arg, .callback_ = recv_callback, .status_ = 0, .length_ = 0, .last_active_ = time(NULL)};
    reactor->ep_selector_.register_event(reactor->events_[cli_fd]);
  } while (0);

  printf("New connect: [%s:%d], [time:%ld], pos[%d]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),
         reactor->events_[cli_fd].last_active_, i);
}

void recv_callback(int fd, int events, void *arg) {
  // 1.获得reactor结构
  Reactor *reactor = static_cast<Reactor *>(arg);
  // 2.获取该fd对应的事件结构
  Event *ev = reactor->events_.data() + fd;

  // 3.先将事件从epoll事件集移除
  reactor->ep_selector_.remove_event(*ev);

  // 3.接收数据
  int rc = recv(ev->fd_, ev->buffer_, MAX_BUFFER_SIZE, 0);
  if (rc < 0)  // recv出错
  {
    // if(errno == EAGAIN || errno == EWOULDBLOCK)
    //     return rc;

    printf("Error in %s(), recv: %s\n", __func__, strerror(errno));

    // 此处我们不再需要将该nty_event从epoll事件集移除, 因为上面我们已经移除了
    close(ev->fd_);
  } else if (rc == 0)  //对方关闭了
  {
    printf("Client closed the connection, fd = %d\n", ev->fd_);

    // 此处我们也当做错误处理
    // 此处我们不再需要将该nty_event从epoll事件集移除, 因为上面我们已经移除了
    close(ev->fd_);
  } else  //接收到数据
  {
    ev->buffer_[rc] = '\0';
    printf("Recv[fd = %d]: %s\n", ev->fd_, ev->buffer_);

    // 将事件变为可读, 然后加入到epoll事件表中
    // *ev = Event{.fd_ = ev->fd_,
    //             .events_ = EPOLLOUT,
    //             .arg_ = arg,
    //             .callback_ = send_callback,
    //             .status_ = 0,
    //             .length_ = rc,
    //             .last_active_ = time(NULL)};
    ev->events_ = EPOLLOUT;
    ev->status_ = 0;
    ev->length_ = rc;
    ev->callback_ = send_callback;
    ev->last_active_ = time(NULL);
    reactor->ep_selector_.register_event(*ev);
  }
}

void send_callback(int fd, int events, void *arg) {
  // 1.获得reactor结构
  Reactor *reactor = static_cast<Reactor *>(arg);
  // 2.获取该fd对应的事件结构
  Event *ev = reactor->events_.data() + fd;

  // 3.此处我们把接收的内容再回送给对象, 因此使用的是ev->buffer
  int rc = send(ev->fd_, ev->buffer_, ev->length_, 0);
  if (rc > 0)  // send成功
  {
    printf("Send[fd = %d]: %s\n", ev->fd_, ev->buffer_);

    // 移除、添加: 将其变为可读
    reactor->ep_selector_.remove_event(*ev);
    *ev = Event{.fd_ = ev->fd_,
                .events_ = EPOLLIN,
                .arg_ = arg,
                .callback_ = recv_callback,
                .status_ = 0,
                .length_ = 0,
                .last_active_ = time(NULL)};
    reactor->ep_selector_.register_event(*ev);
  } else  // send失败
  {
    printf("Error in %s(), send: %s\n", __func__, strerror(errno));

    // 关闭、移除
    close(ev->fd_);
    reactor->ep_selector_.register_event(*ev);
  }
}
}  // namespace reactor

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "usage: %s ip port\n" << argv[0] << std::endl;
    exit(1);
  }

  reactor::Reactor server(argv[1], atoi(argv[2]));
  server.set_accept_callback(reactor::accept_callback);
  server.set_recv_callback(reactor::recv_callback);
  server.set_send_callback(reactor::send_callback);

  server.add_listener();
  server.run();

  return 0;
}
