#pragma once

#include "Decorator.h"

namespace bt {

class Inverter : public Decorator {
public:

    explicit Inverter(const std::string& name)
    {
        name_ = name;
    }
    
    Inverter() : Inverter("Inverter") {}

    Status tick() override;
};
}
