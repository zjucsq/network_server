#pragma once

#include <functional>
#include <vector>

#include "epoll_selector.h"
#include "event.h"
#include "tcp_conn.h"

namespace reactor {
class Reactor {
 public:
  using accept_callback = std::function<void(int, int, void*)>;
  using recv_callback = std::function<void(int, int, void*)>;
  using send_callback = std::function<void(int, int, void*)>;

  Reactor(std::string ip, int port);

  void run();

  void add_listener();

  void set_accept_callback(const accept_callback& acb) { acb_ = acb; }
  void set_recv_callback(const recv_callback& rcb) { rcb_ = rcb; }
  void set_send_callback(const send_callback& scb) { scb_ = scb; }

  EpollSelector ep_selector_;
  TcpConn tcp_conn_;
  std::vector<Event> events_;
  int max_epoll_events_{1024};

 private:
  accept_callback acb_;
  recv_callback rcb_;
  send_callback scb_;
};
}  // namespace reactor