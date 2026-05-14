#include "bt/Sequence.h"
#include <iostream>
namespace bt {

Status Sequence::tick() {

    // And 조건
    for (const auto& child : children_) {
        Status status = child->tick();
        
        if (status == Status::Failure) {
            return Status::Failure;
        }

        if (status == Status::Running) {
            return Status::Running;
        }
    }
    
    return Status::Success;
}

} // namespace bt
