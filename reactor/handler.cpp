#include "handler.h"

namespace reactor {

Handler::Handler() : handler_id_(handler_id_generation_++) {}

HandlerId Handler::handler_id_generation_ = 0;

}  // namespace reactor