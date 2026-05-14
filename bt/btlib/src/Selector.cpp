#include "bt/Selector.h"

namespace bt {

Status Selector::tick() {
    
    // Or 조건
    for (const auto& child : children_) {
        Status status = child->tick();

        if (status == Status::Success) {
            return Status::Success;
        }

        if (status == Status::Running) {
            return Status::Running;
        }
    }
    
    return Status::Failure;
}

} // namespace bt
