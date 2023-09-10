#pragma once

#include <cstdint>

namespace reactor {
using HandlerId = uint64_t;

class Handler {
  Handler();

 private:
  HandlerId handler_id_;
  static HandlerId handler_id_generation_;
};
}  // namespace reactor