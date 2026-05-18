#pragma once

#include "Composite.h"

namespace bt {

class Selector : public Composite {
public:
    Status tick() override;
    std::string name() const override { return "Selector"; }
};

} // namespace bt
