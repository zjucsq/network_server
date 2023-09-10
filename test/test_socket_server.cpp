#include "socket_server.h"

int main() {
  SocketServer server(19234, 1024);
  server.init();
  server.start();
}