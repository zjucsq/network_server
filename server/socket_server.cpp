#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

const int BUFFER_SIZE = 1024;
const int PORT = 19234;

int main() {
  // 创建 socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cerr << "Failed to create socket." << std::endl;
    return -1;
  }

  // 设置服务器地址和端口
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT);

  // 绑定 socket
  if (bind(serverSocket, (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) == -1) {
    std::cerr << "Failed to bind socket." << std::endl;
    return -1;
  }

  // 监听连接
  if (listen(serverSocket, 5) == -1) {
    std::cerr << "Failed to listen." << std::endl;
    return -1;
  }

  std::cout << "Server started. Listening on port " << PORT << "..."
            << std::endl;

  while (true) {
    // 接受客户端连接
    sockaddr_in clientAddress{};
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress,
                              &clientAddressSize);
    if (clientSocket == -1) {
      std::cerr << "Failed to accept client connection." << std::endl;
      continue;
    }

    std::cout << "Client connected: " << inet_ntoa(clientAddress.sin_addr)
              << ":" << ntohs(clientAddress.sin_port) << std::endl;

    // 接收和发送数据
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
      std::cout << "Received data: " << buffer << std::endl;
      ssize_t bytesSent = send(clientSocket, buffer, bytesRead, 0);
      if (bytesSent != bytesRead) {
        std::cerr << "Failed to send data back to client." << std::endl;
        break;
      }
    }

    // 关闭客户端连接
    close(clientSocket);

    std::cout << "Client disconnected: " << inet_ntoa(clientAddress.sin_addr)
              << ":" << ntohs(clientAddress.sin_port) << std::endl;
  }

  // 关闭服务器 socket
  close(serverSocket);

  return 0;
}
