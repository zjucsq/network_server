#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <stdexcept>

class EpollServer {
  int serverSocket;
  int epollFd;
  epoll_event event;
  int port;
  int bufferSize;
  int maxEvents;

 public:
  EpollServer(int port, int buffer, int maxEvents) : serverSocket(-1), epollFd(-1), port(port), bufferSize(buffer), maxEvents(maxEvents) {}

  void init() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) std::runtime_error("Failed to create server socket");

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) std::runtime_error("Failed to bind socket");

    if (listen(serverSocket, 5) == -1) std::runtime_error("Failed to bind listen");

    epollFd = epoll_create1(0);
    if (epollFd < 0) std::runtime_error("Failed to create epoll");

    event.data.fd = serverSocket;
    event.events = EPOLLIN;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) < 0) std::runtime_error("Failed to add listen socket to epoll");

    std::cout << "Server started. Listening on port " << port << "..." << std::endl;
  }

  void start() {
    epoll_event events[maxEvents];

    while (true) {
      // 等待事件
      int numEvents = epoll_wait(epollFd, events, maxEvents, -1);
      if (numEvents < 0) {
        std::cerr << "Failed to wait for events" << std::endl;
      }

      // 处理每个事件
      for (int i = 0; i < numEvents; i++) {
        std::cout << i;
        int fd = events[i].data.fd;

        // 如果是监听socket，则有新连接
        if (fd == serverSocket) {
          sockaddr_in clientAddress{};
          socklen_t clientAddressLength = sizeof(clientAddress);
          int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
          if (clientSocket < 0) {
            std::cerr << "Failed to accept new connection" << std::endl;
            continue;
          }

          // 将新连接的socket添加到epoll
          event.data.fd = clientSocket;
          event.events = EPOLLIN;
          if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) < 0) {
            std::cerr << "Failed to add client socket to epoll" << std::endl;
          }

          std::cout << "New client connected: " << inet_ntoa(clientAddress.sin_addr) << std::endl;
        }
        // 否则是已连接socket上有数据可读
        else {
          char buffer[bufferSize];
          memset(buffer, 0, sizeof(buffer));

          ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);
          if (bytesRead <= 0) {
            // 客户端断开连接
            close(fd);
            epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);
            std::cout << "Client disconnected" << std::endl;
          } else {
            std::cout << "Received data: "
                      << "size=" << bytesRead << ' ' << buffer << std::endl;
            send(fd, buffer, bytesRead, 0);
          }
        }
      }
    };
  }
  void stop() {
    close(serverSocket);
    close(epollFd);
  }
};