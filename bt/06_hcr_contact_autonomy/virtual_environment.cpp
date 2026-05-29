#include "virtual_environment.h"

namespace ve {

void VirtualEnv::update(const State& target)
{
    // update position
    curr_state.position = target.position;

    // contact check
    is_contact = curr_state.position <= object_pose_z;
    curr_state.force = 0.0;
    if(is_contact)
    {
        curr_state.force = target.force;
    }
}

} // namespace ve
