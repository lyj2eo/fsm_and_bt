#pragma once
#include "Node.h"
#include <memory>

namespace bt {

class Decorator : public Node {
public:
    void addChild(std::unique_ptr<Node> child);

    std::unique_ptr<Node>* getChild() override
    {
        if(!child_){
            return nullptr;
        }
        return &child_;
    }
protected:
    std::unique_ptr<Node> child_;
};    

} // namespace bt