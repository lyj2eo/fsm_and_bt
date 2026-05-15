#pragma once
#include "Node.h"
#include <memory>

namespace bt {

class Decorator : public Node {
public:
    void setChild(std::unique_ptr<Node> child);

protected:
    std::unique_ptr<Node> child_;
};    

}