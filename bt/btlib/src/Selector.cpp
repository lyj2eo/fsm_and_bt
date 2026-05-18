#include "bt/Selector.h"

namespace bt {

Status Selector::tick() {
    
    // Or 조건
    // for (const auto& child : children_) {
    for (size_t i = child_idx_; i < children_.size(); ++i) {
        Status status = children_[i]->tick();

        if (status == Status::Success) {
            child_idx_ = 0;
            return Status::Success;
        }

        if (status == Status::Running) {
            child_idx_ = i;
            return Status::Running;
        }
    }
    
    child_idx_ = 0;
    return Status::Failure;
}

} // namespace bt
