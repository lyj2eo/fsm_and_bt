#pragma once

#include "contact_supervisor.h"
#include "../context/contact_context.h"

namespace ca {

bt::Sequence Supervisor::makeScenario()
{
    if( std::string(ca::CONTACT_TASK) == "contact_detection" )
    {
        bt::Sequence root(ca::CONTACT_TASK, bt::Sequence::SequenceType::Resume);
    
        root.addChild(std::make_unique<contact::EnableForceMode>(ctx_));
        root.addChild(std::make_unique<contact::Wait>(ctx_, bb_));
        root.addChild(std::make_unique<contact::DisableForceMode>(ctx_));
        root.setBlackboard(bb_);
    
        return root;
    }

    return bt::Sequence(ca::CONTACT_TASK, bt::Sequence::SequenceType::Resume);
}

} // namespace ca
