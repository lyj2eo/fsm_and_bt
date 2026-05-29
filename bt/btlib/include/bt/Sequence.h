#pragma once

#include "Composite.h"

namespace bt {

class Sequence : public Composite {
public:
    enum class SequenceType{ Restart, Resume };

    Sequence(const std::string& name, SequenceType st = SequenceType::Resume): st_(st)
    {
        name_ = name;
    }
    
    Sequence() : Sequence("Sequence") {}

    Status tick() override;

private:
    SequenceType st_;
};

} // namespace bt
