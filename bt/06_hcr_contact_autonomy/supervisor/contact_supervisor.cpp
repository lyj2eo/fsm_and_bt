#pragma once

#include "contact_supervisor.h"
#include "../context/contact_context.h"

namespace ca {

Supervisor::Decision Supervisor::evaluate() 
{   
    // abort case
    if (0) {
        return Decision::Abort;
    }

    // recovery case
    if (0) {
        return Decision::Recovery;
    }

    // idle case
    if (0) {
        return Decision::Idle;
    }

    return Decision::Proceed;
}

// bt::Status Supervisor::tick()
// {
//     const Decision decision = evaluate();
//     // auto root = makeScenario(decision);
//     return root.tick();
// }

bt::Sequence Supervisor::makeScenario()
{
    // if (decision == Decision::Abort)
    // {
    //     return makeAbortScenario();
    // }

    if( std::string(ca::CONTACT_TASK) == "contact_detection" )
    {
        return makeDetectContactScenario();

    } else if (std::string(ca::CONTACT_TASK) == "static_pressing") 
    {
        return makeStaticPressingScenario();
    }
    
    return makeAbortScenario();

}


bt::Sequence Supervisor::makeDetectContactScenario()
{
    bt::Sequence root(ca::CONTACT_TASK, bt::Sequence::SequenceType::Resume);
    
    root.addChild(std::make_unique<contact::EnableForceMode>(ctx_));
    root.addChild(std::make_unique<contact::Wait>(ctx_, bb_));
    root.addChild(std::make_unique<contact::DisableForceMode>(ctx_));
    root.setBlackboard(bb_);
    
    return root;
}

bt::Sequence Supervisor::makeStaticPressingScenario()
{
    bt::Sequence root(ca::CONTACT_TASK, bt::Sequence::SequenceType::Resume);
    
    
    auto pressing_success_seq = std::make_unique<bt::Sequence>("pressing_success_seq");
    auto pressing_result_sel = std::make_unique<bt::Selector>("pressing_result_sel");
    auto pressing_seq = std::make_unique<bt::Sequence>("pressing_seq");
    auto contact_and_press_seq = std::make_unique<bt::Sequence>("contact_and_press_seq");
    auto contact_or_fail_sel = std::make_unique<bt::Selector>("contact_or_fail_sel");

    pressing_success_seq->addChild(std::make_unique<contact::Pressing>(task_input_, bb_));
    pressing_success_seq->addChild(std::make_unique<contact::SetTaskSuccess>(bb_));

    pressing_result_sel->addChild(std::move(pressing_success_seq));
    pressing_result_sel->addChild(std::make_unique<contact::SetTaskFail>(bb_));

    pressing_seq->addChild(std::move(pressing_result_sel));
    pressing_seq->addChild(std::make_unique<contact::MoveBackward>(ctx_, bb_));
    
    contact_and_press_seq->addChild(std::make_unique<contact::MoveUntilContact>(task_input_, bb_));
    contact_and_press_seq->addChild(std::move(pressing_seq));

    contact_or_fail_sel->addChild(std::move(contact_and_press_seq));
    contact_or_fail_sel->addChild(std::make_unique<contact::SetTaskFail>(bb_));
    
    root.addChild(std::make_unique<contact::EnableForceMode>(ctx_));
    root.addChild(std::move(contact_or_fail_sel));
    root.addChild(std::make_unique<contact::DisableForceMode>(ctx_));
    
    root.setBlackboard(bb_);

    return root;

}

bt::Sequence Supervisor::makeAbortScenario()
{
    bt::Sequence root(ca::CONTACT_TASK, bt::Sequence::SequenceType::Resume);
    
    root.addChild(std::make_unique<contact::EnableForceMode>(ctx_));
    root.setBlackboard(bb_);
    
    return root;
}

} // namespace ca
