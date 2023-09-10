#pragma once

#include <string>

namespace reactor {
class TcpConn {
  friend class Reactor;

 public:
  TcpConn(std::string ip, int port);

  int sock_fd;

 private:
  std::string ip_;
  int port_;
};
}  // namespace reactor