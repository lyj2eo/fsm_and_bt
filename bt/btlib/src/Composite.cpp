#include "bt/Composite.h"

namespace bt {

void Composite::addChild(std::unique_ptr<Node> child) 
{
    if(bb_){
        child->setBlackboard(*bb_);
    }
    children_.push_back(std::move(child));
}

} // namespace bt
