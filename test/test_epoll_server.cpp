#include "epoll_server.h"

int main() {
  EpollServer server(19235, 1024, 1);
  server.init();
  server.start();
}