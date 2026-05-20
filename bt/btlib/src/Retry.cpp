#include "bt/Retry.h"
#include <iostream>

namespace bt{

Status Retry::tick()
{
    if (!child_) {
        std::cout << "[Retry : "<< name_ << "] has no child node\n";
        return Status::Failure;
    }
    
    Status status = child_->tick();
    // success
    if (status == Status::Success){
        std::cout << "[Retry : "<< name_ << "] ==== Success ===="<<std::endl;
        attempts_ = 0;
        return Status::Success;
    } 

    // running
    if (status == Status::Running) {
        std::cout << "[Retry : "<< name_ << "] . . . Running . . ."<<std::endl;
        return Status::Running;
    }

    // failure
    attempts_++;    
    if (attempts_ >= max_attempts_) 
    {
        attempts_ = 0;
        std::cout << "[Retry : "<< name_ << "] ==== Failure ===="<<std::endl;
        return Status::Failure;
    }

    return Status::Running;
}
} // namespace bt