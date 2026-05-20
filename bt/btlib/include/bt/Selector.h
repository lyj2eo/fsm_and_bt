#pragma once

#include "Composite.h"

namespace bt {

class Selector : public Composite {
public:

    Selector(const std::string& name) 
    {
        name_ = name;
    }
    
    Selector() : Selector("Selector") {}

    Status tick() override;
};

} // namespace bt
