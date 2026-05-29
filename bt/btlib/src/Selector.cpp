#include "bt/Selector.h"
#include <iostream>

namespace bt {

Status Selector::tick() {
    
    // resume indexing for running Selector child
    for (size_t i = child_idx_; i < children_.size(); ++i) {
        Status status = children_[i]->tick();

        if (status == Status::Success) {
            child_idx_ = 0;
            // std::cout<<"[Selector : "<< name_ << "] ==== Success ===="<<std::endl;
            return Status::Success;
        }

        if (status == Status::Running) {
            child_idx_ = i;
            std::cout<<"[Selector : "<< name_ << "] . . . Running . . ."<<std::endl;
            return Status::Running;
        }
    }
    
    child_idx_ = 0;
    std::cout<<"[Selector : "<< name_ << "] ==== Failure ===="<<std::endl;
    return Status::Failure;
}

} // namespace bt
