#pragma once

#include <vector>

#include "net/event.h"
#include "noncopyable.h"

namespace asyncio {
struct Stream : NonCopyable {
  using Buffer = std::vector<char>;
  Stream(int fd) {}

 private:
  int read_fd_{-1};
  int write_fd_{-1};
  Event read_ev_{.fd = read_fd_, .flags = Event::Flags::EVENT_READ};
  Event write_ev_{.fd = write_fd_, .flags = Event::Flags::EVENT_WRITE};
};
}  // namespace asyncio