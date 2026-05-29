#pragma once

#include "../context/contact_context.h"
#include "bt/Blackboard.h"
#include "bt/Sequence.h"
#include "bt/Selector.h"

#include "../actions/contact_detection_actions.h"
#include "../actions/contact_detection_actions.h"

#include <vector>

namespace ca {

class Supervisor{
public:

    Supervisor(RobotContext& ctx, TaskInput& task_input, bt::Blackboard& bb)
        : ctx_(ctx), task_input_(task_input), bb_(bb) {}

    enum class Decision { Idle, Proceed, Recovery, Abort };

    bt::Sequence makeScenario();
    
    Decision evaluate();
    bt::Status tick();
    
    bt::Sequence makeDetectContactScenario();
    bt::Sequence makeStaticPressingScenario();
    bt::Sequence makeAbortScenario();

private:
    RobotContext& ctx_;
    TaskInput& task_input_;
    bt::Blackboard& bb_;
    
    Decision active_decision_ = Decision::Idle;
    // Decision prev_decision_ = Decision::Idle;
    std::unique_ptr<bt::Sequence> active_scenario_;
};

} // namespace ca
