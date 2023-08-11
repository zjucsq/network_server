#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

const int BUFFER_SIZE = 1024;
const int PORT = 19234;

void sendRequest(int threadId) {
  // 创建socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    std::cerr << "Failed to create socket" << std::endl;
    return;
  }

  // 连接服务器
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
  serverAddress.sin_port = htons(PORT);
  if (connect(clientSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) < 0) {
    std::cerr << "Failed to connect to server" << std::endl;
    close(clientSocket);
    return;
  }

  // 构造请求数据
  std::string request = "Hello from thread " + std::to_string(threadId);

  // 发送请求
  ssize_t bytesSent =
      send(clientSocket, request.c_str(), request.length() + 1, 0);
  if (bytesSent < 0) {
    std::cerr << "Failed to send request" << std::endl;
    close(clientSocket);
    return;
  }

  // 接收响应
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, sizeof(buffer));
  ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead < 0) {
    std::cerr << "Failed to receive response" << std::endl;
  } else {
    std::cout << "Received response: " << buffer << std::endl;
  }

  // 关闭socket
  std::this_thread::sleep_for(std::chrono::seconds(2));
  close(clientSocket);
}

int main() {
  const int NUM_THREADS = 10;
  std::vector<std::thread> threads;

  // 创建多个线程来发送请求
  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(std::thread(sendRequest, i));
  }

  // 等待所有线程完成
  for (auto &thread : threads) {
    thread.join();
  }

  return 0;
}
