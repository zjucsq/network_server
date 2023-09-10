#include "tcp_conn.h"

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstdlib>
#include <cstring>

namespace reactor {
TcpConn::TcpConn(std::string ip, int port) {
  // 1.创建套接字
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    printf("Error in %s(), socket: %s\n", __func__, strerror(errno));
    exit(1);
  }

  // 2.初始化服务器地址
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  if (inet_pton(AF_INET, ip.c_str(), (void *)&server_addr.sin_addr.s_addr) == -1) {
    printf("Error in %s(), inet_pton: %s\n", __func__, strerror(errno));
    exit(1);
  }
  server_addr.sin_port = htons(port);

  // 3.绑定服务器地址
  if (bind(sock_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    printf("Error in %s(), bind: %s\n", __func__, strerror(errno));
    exit(1);
  }

  // 3.监听
  if (listen(sock_fd, 20) == -1) {
    printf("Error in %s(), listen: %s\n", __func__, strerror(errno));
    exit(1);
  }

  printf("Listen start [%s:%d]...\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
}
}  // namespace reactor