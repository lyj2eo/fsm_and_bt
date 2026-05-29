#pragma once

#include "bt/Status.h"
#include "observer/observer.h"

#include <vector>

namespace ve {

struct State
{
    double position = 0.0;
    double force = 0.0;
};


struct VirtualEnv
{
    VirtualEnv() {}

    void update(const State& target);

    double object_pose_z = -0.1;

    State curr_state;
    bool is_contact = false;
};

} // namespace ve
