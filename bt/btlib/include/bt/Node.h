#pragma once

#include "Status.h"
#include "Blackboard.h"
#include <memory>

#include <string>

namespace bt {

class Node {

public:
    virtual ~Node() = default;

    virtual Status tick() = 0;
    const std::string& name() const {return name_;}
    virtual std::vector<std::unique_ptr<Node>>* getChildren(){ return nullptr; };
    virtual std::unique_ptr<Node>* getChild(){ return nullptr; };

    void setBlackboard(Blackboard& bb)
    {
        auto get_children = getChildren();
        auto get_child    = getChild();

        if(get_children)
        {   // all children of composite(seq, sel, dec ...) share the same blackboard
            for (auto& child : *get_children) {
                child->setBlackboard(bb);
            }

        } else if(get_child)
        {   // single child of decorator share the same blackboard
            (*get_child)->setBlackboard(bb);

        } else
        {   // leaf node(action ..) also share the same blackboard
            bb_ = &bb;
        }
    }
    
protected:
    explicit Node(const std::string& name) : name_(name) {}
    Node() = default;

    Blackboard* bb_ = nullptr;
    std::string name_{""};

};

} // namespace bt
