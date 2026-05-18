#include "bt/Sequence.h"
#include <iostream>
namespace bt {

Status Sequence::tick() {
    // SequenceType
    size_t start_idx = (st_ == SequenceType::Restart)? 0 : child_idx_;
    
    for (size_t i = start_idx; i < children_.size(); ++i) {
        Status status = children_[i]->tick();

        if (status == Status::Running) {
            child_idx_ = i;
            std::cout<<"[Sequence] . . . Running . . ."<<std::endl;
            return Status::Running;
        }
        
        if (status == Status::Failure) {
            child_idx_ = 0;
            std::cout<<"[Sequence] ==== Failure ===="<<std::endl;
            return Status::Failure;
        }
    }
    
    child_idx_ = 0;
    std::cout<<"[Sequence] ==== Success ===="<<std::endl;
    return Status::Success;
}

} // namespace bt
