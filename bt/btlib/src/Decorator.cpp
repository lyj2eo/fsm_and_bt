#include "bt/Decorator.h"

namespace bt{
    
void Decorator::setChild(std::unique_ptr<Node> child)
{
    child_ = std::move(child);
};
} // namespace bt