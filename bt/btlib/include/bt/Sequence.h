#pragma once

#include "Composite.h"

namespace bt {

class Sequence : public Composite {
public:
    enum class SequenceType{ Restart, Resume };

    Sequence(SequenceType st = SequenceType::Restart)
        : st_(st){}

    Status tick() override;
    std::string name() const override { return "Sequence"; }

private:
    SequenceType st_;
};

} // namespace bt
