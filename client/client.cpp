#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <cxxopts.hpp>
#include <iostream>
#include <thread>
#include <vector>

auto parse(int argc, const char *argv[]) {
  cxxopts::Options options(argv[0], "Client");
  options.allow_unrecognised_options();
  options.show_positional_help();
  options.set_width(70).set_tab_expansion().add_options("MyOption")(
      "i,ip", "ip address",
      cxxopts::value<std::string>()->implicit_value("127.0.0.1"))(
      "p,port", "post", cxxopts::value<int>()->implicit_value("19234"))(
      "b,buffer", "buffer size", cxxopts::value<int>()->implicit_value("1024"))(
      "help", "print help");

  auto result = options.parse(argc, argv);
  return result;
}

int main(int argc, const char *argv[]) {
  auto args = parse(argc, argv);

  // 创建socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    std::cerr << "Failed to create socket" << std::endl;
    return 0;
  }

  // 连接服务器
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
  serverAddress.sin_port = htons(args["p"].as<int>());
  if (connect(clientSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) < 0) {
    std::cerr << "Failed to connect to server" << std::endl;
    close(clientSocket);
    return 0;
  }

  std::string request;
  while (true) {
    std::cin >> request;

    // 发送请求
    ssize_t bytesSent =
        send(clientSocket, request.c_str(), request.length() + 1, 0);
    if (bytesSent < 0) {
      std::cerr << "Failed to send request" << std::endl;
      close(clientSocket);
      return 0;
    }
  }

  // 接收响应
  char buffer[args["b"].as<int>()];
  memset(buffer, 0, sizeof(buffer));
  ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead < 0) {
    std::cerr << "Failed to receive response" << std::endl;
  } else {
    std::cout << "Received response: " << buffer << std::endl;
  }

  close(clientSocket);
  return 0;
}
