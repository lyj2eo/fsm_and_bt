#include "observer.h"

#include <iostream>

namespace ca {

Observer::Observer(RobotContext& ctx, bt::Blackboard& bb)
    : ctx_(ctx),
      bb_(bb)
{
}

void Observer::init()
{
    // clink -> observer variables initialize
    fts_.force = std::vector<double>(6, 0.0);
    fts_.torque = std::vector<double>(6, 0.0);
    rs_.joint_position = std::vector<double>(6, 0.0);
    rs_.joint_velocity = std::vector<double>(6, 0.0);
    rs_.tcp_position = std::vector<double>(3, 0.0);
    rs_.tcp_orientation = std::vector<double>(3, 0.0);

    // set to blackboard
    setToBlackboard();
}

void Observer::update()
{
    // clink -> observer update (TODO)
    if (ctx_.force_mode_enabled) {
        fts_.force[2] += 2.5;
    }
    // fts_.torque = ctx_.fts_torque;
    
    std::cout << "[Observer] force_z=" << fts_.force[2] << "\n";
    
    // blackboard update 
    bb_.update("fts_force", fts_.force);
}

void Observer::setToBlackboard()
{
    // fts
    bb_.set("fts_force", fts_.force);
    bb_.set("fts_torque", fts_.torque);

    // robot states
    bb_.set("joint_position", rs_.joint_position);
    bb_.set("joint_velocity", rs_.joint_velocity);
    bb_.set("tcp_position", rs_.tcp_position);
    bb_.set("tcp_orientation", rs_.tcp_orientation);
}

} // namespace ca
