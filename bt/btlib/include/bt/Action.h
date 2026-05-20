#pragma once

#include "Node.h"

#include <functional>
#include <string>
#include <utility>

namespace bt {

class Action : public Node {
public:
    using Func = std::function<Status()>;

    Action(const std::string& name, Func func) 
    : func_(std::move(func))
    {  
        name_ = name;
    }
    
    Action(const std::string& name) 
    {  
        name_ = name;
    }
    
    Status tick() override;

private:
    Func func_;
};

} // namespace bt
