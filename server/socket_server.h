#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <stdexcept>

class SocketServer {
  int serverSocket;
  int port;
  int bufferSize;

 public:
  SocketServer(int port, int buffer)
      : serverSocket(-1), port(port), bufferSize(buffer) {}

  void init() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
      std::runtime_error("Failed to create server socket");

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) == -1)
      std::runtime_error("Failed to bind socket");

    if (listen(serverSocket, 5) == -1)
      std::runtime_error("Failed to bind listen");

    std::cout << "Server started. Listening on port " << port << "..."
              << std::endl;
  }

  void start() {
    while (true) {
      sockaddr_in clientAddress{};
      socklen_t clientAddressSize = sizeof(clientAddress);
      int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress,
                                &clientAddressSize);
      if (clientSocket == -1)
        std::runtime_error("Failed to accept client connection.");

      std::cout << "Client connected: " << inet_ntoa(clientAddress.sin_addr)
                << ":" << ntohs(clientAddress.sin_port) << std::endl;

      char buffer[bufferSize];
      ssize_t bytesRead;
      while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        std::cout << "Received data: " << buffer << std::endl;
        ssize_t bytesSent = send(clientSocket, buffer, bytesRead, 0);
        if (bytesSent != bytesRead)
          std::runtime_error("Failed to send data back to client.");
      }

      close(clientSocket);

      std::cout << "Client disconnected: " << inet_ntoa(clientAddress.sin_addr)
                << ":" << ntohs(clientAddress.sin_port) << std::endl;
    }
  };

  void stop() { close(serverSocket); }
};