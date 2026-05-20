#include "bt/Decorator.h"

namespace bt{
    
void Decorator::addChild(std::unique_ptr<Node> child)
{
    if(bb_){
        child->setBlackboard(*bb_);
    }
    
    child_ = std::move(child);
};
} // namespace bt