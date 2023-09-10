#pragma once

#include <sys/socket.h>

#include <cerrno>

namespace net {
void connect(int fd, const struct sockaddr *addr, socklen_t len) {
  int rc = ::connect(fd, addr, len);
  if (rc == 0) return;
  // EINPROGRESS代表连接还在进行中，后面可以通过poll或者select来判断socket是否可写，如果可以写，说明连接完成了
  if (rc < 0 && errno != EINPROGRESS) }
};  // namespace net
