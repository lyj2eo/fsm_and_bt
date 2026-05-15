#include "bt/Retry.h"

namespace bt{

Status Retry::tick()
{
    if (!child_) {
        return Status::Failure;
    }
    
    Status status = child_->tick();
    // success
    if (status == Status::Success){
        attempts_ = 0;
        return Status::Success;
    } 

    // running
    if (status == Status::Running) {
        return Status::Running;
    }

    // failure
    attempts_++;    
    if (attempts_ >= max_attempts_) 
    {
        attempts_ = 0;
        return Status::Failure;
    }

    return Status::Running;   

}
} // namespace bt