#include "bt/Action.h"

namespace bt {

Status Action::tick() {
    return func_ ? func_() : Status::Failure;
}

} // namespace bt
