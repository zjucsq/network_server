#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <vector>

const int MAX_EVENTS = 1;
const int BUFFER_SIZE = 1024;
const int PORT = 19234;

int main() {
  // 创建监听socket
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    std::cerr << "Failed to create socket" << std::endl;
    return -1;
  }

  // 绑定IP和端口
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT);
  if (bind(listenSocket, (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) < 0) {
    std::cerr << "Failed to bind" << std::endl;
    return -1;
  }

  // 开始监听
  if (listen(listenSocket, 5) < 0) {
    std::cerr << "Failed to listen" << std::endl;
    return -1;
  }

  // 创建epoll实例
  int epollFd = epoll_create1(0);
  if (epollFd < 0) {
    std::cerr << "Failed to create epoll" << std::endl;
    return -1;
  }

  // 添加监听socket到epoll
  epoll_event event{};
  event.data.fd = listenSocket;
  event.events = EPOLLIN;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, listenSocket, &event) < 0) {
    std::cerr << "Failed to add listen socket to epoll" << std::endl;
    return -1;
  }

  // 创建事件数组
  std::vector<epoll_event> events(MAX_EVENTS);

  while (true) {
    // 等待事件
    int numEvents = epoll_wait(epollFd, events.data(), MAX_EVENTS, -1);
    if (numEvents < 0) {
      std::cerr << "Failed to wait for events" << std::endl;
      return -1;
    }

    // 处理每个事件
    for (int i = 0; i < numEvents; i++) {
      int fd = events[i].data.fd;

      // 如果是监听socket，则有新连接
      if (fd == listenSocket) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket =
            accept(listenSocket, (struct sockaddr *)&clientAddress,
                   &clientAddressLength);
        if (clientSocket < 0) {
          std::cerr << "Failed to accept new connection" << std::endl;
          continue;
        }

        // 将新连接的socket添加到epoll
        event.data.fd = clientSocket;
        event.events = EPOLLIN;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) < 0) {
          std::cerr << "Failed to add client socket to epoll" << std::endl;
          return -1;
        }

        std::cout << "New client connected: "
                  << inet_ntoa(clientAddress.sin_addr) << std::endl;
      }
      // 否则是已连接socket上有数据可读
      else {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
          // 客户端断开连接
          close(fd);
          epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);
          std::cout << "Client disconnected" << std::endl;
        } else {
          send(fd, buffer, bytesRead, 0);
          std::cout << "Received data: " << buffer << std::endl;
        }
      }
    }
  }

  // 清理资源
  close(listenSocket);
  close(epollFd);

  return 0;
}
