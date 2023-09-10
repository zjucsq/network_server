#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>

#define SERVER_ADDRESS "127.0.0.1"

int BUFFER_SIZE = 1024;
int SERVER_PORT = 8888;
int mseconds = 20;

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  // https://blog.csdn.net/afei__/article/details/81261879
  int o;
  const char *optstring = "b::p::m::";  // 没有冒号表示不带值的参数，一个冒号表示必须带值，两个冒号表示后面可选参数
  while ((o = getopt(argc, argv, optstring)) != -1) {
    switch (o) {
      case 'b':
        BUFFER_SIZE = atoi(optarg);
        break;
      case 'p':
        SERVER_PORT = atoi(optarg);
        break;
      case 'm':
        mseconds = atoi(optarg);
        break;
      case '?':
        printf("解析错误\n");
        // usage(); // 提示使用说明
        break;
    }
  }

  std::cout << "set buffer size = " << BUFFER_SIZE << std::endl;
  std::cout << "set server port = " << SERVER_PORT << std::endl;
  std::cout << "sleep milliseconds = " << mseconds << std::endl;

  int sock;
  struct sockaddr_in serverAddress;
  char buffer[BUFFER_SIZE] = {'h', 'e', 'l', 'l', 'o', '\0'};
  char recv_buffer[BUFFER_SIZE] = {0};

  // 创建套接字
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    error("Failed to create socket");
  }

  // 设置服务器地址
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(SERVER_PORT);
  if (inet_pton(AF_INET, SERVER_ADDRESS, &serverAddress.sin_addr) <= 0) {
    error("Failed to setup server address");
  }

  // 连接到服务器
  if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
    error("Failed to connect to server");
  }

  // 发送和接收数据
  struct timeval start, end;
  gettimeofday(&start, NULL);

  for (int i = 0; i < 100; i++) {
    int sendBytes = send(sock, buffer, BUFFER_SIZE, 0);
    if (sendBytes < 0) {
      error("Failed to send data");
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(mseconds));
    memset(recv_buffer, 0, sizeof(recv_buffer));
    int recvBytes = recv(sock, recv_buffer, BUFFER_SIZE, 0);
    std::cout << "Received data: "
              << "size=" << recvBytes << ' ' << recv_buffer << std::endl;
    if (recvBytes < 0) {
      error("Failed to receive data");
    }
  }

  gettimeofday(&end, NULL);

  // 计算时间差并打印性能测试结果
  double elapsedTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
  std::cout << "Elapsed time: " << elapsedTime << " seconds" << std::endl;
  std::cout << "Requests per second: " << 10000 / elapsedTime << std::endl;

  // 关闭套接字
  close(sock);

  return 0;
}
