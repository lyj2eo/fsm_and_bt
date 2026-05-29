#pragma once

#include "contact_state.h"
#include "../context/contact_context.h"
#include "bt/Blackboard.h"

namespace ca {

class Identifier {
public:

    Identifier(bt::Blackboard& bb);

    void init();
    void update();

    
    bool isContact()  const;
    void EventIdentifier();

private:
    bt::Blackboard& bb_;
    ContactEvent contact_event_;
    ContactMode contact_mode_;
    std::string cur_event_, cur_mode_, prev_event_, prev_mode_;
};

} // namespace ca
