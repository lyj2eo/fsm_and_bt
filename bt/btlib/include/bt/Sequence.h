#pragma once

#include "Composite.h"

namespace bt {

class Sequence : public Composite {
public:
    Status tick() override;
    std::string name() const override { return "Sequence"; }
};

} // namespace bt
