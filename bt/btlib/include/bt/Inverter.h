#pragma once

#include "Decorator.h"

namespace bt {

class Inverter : public Decorator {
public:
    Status tick() override;
    std::string name() const override { return "Inverter"; }
};
}
