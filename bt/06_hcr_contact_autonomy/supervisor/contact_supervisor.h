#pragma once

#include "../context/contact_context.h"
#include "bt/Blackboard.h"
#include "bt/Sequence.h"

#include "../actions/contact_detection_actions.h"

#include <vector>

namespace ca {

class Supervisor{
public:

    Supervisor(RobotContext& ctx, bt::Blackboard& bb)
        : ctx_(ctx), bb_(bb) {}

    // enum class Decision { Proceed, Recovery, Abort };

    bt::Sequence makeScenario();

private:
    RobotContext& ctx_;
    bt::Blackboard& bb_;
    

    // Decision lastDecision_ = Decision::Proceed;
};

} // namespace ca
