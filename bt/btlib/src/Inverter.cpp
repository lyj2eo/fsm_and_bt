#include "bt/Inverter.h"

namespace bt {

Status Inverter::tick() 
{ 
    if (!child_) {
        return Status::Failure;
    }

    auto status = child_->tick();
    if(status == Status::Success){
        return Status::Failure;
    }

    if(status == Status::Failure){
        return Status::Success;
    }    

    return Status::Running;
}
} // namespace bt