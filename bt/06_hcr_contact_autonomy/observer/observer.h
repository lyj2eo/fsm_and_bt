#pragma once

#include "../context/contact_context.h"
#include "bt/Blackboard.h"

#include <list>
#include <string>
#include <type_traits>
#include <utility>

namespace ca {

class Observer {
public:
    Observer(RobotContext& ctx, bt::Blackboard& bb);

    void init();
    void update();
    void setToBlackboard();

    struct ForceTorqueSensor
    {
        std::vector<double> force = std::vector<double>(3, 0.0);
        std::vector<double> torque = std::vector<double>(3, 0.0);
    };
    
    struct RobotState
    {
        std::vector<double> joint_position = std::vector<double>(6, 0.0);
        std::vector<double> joint_velocity = std::vector<double>(6, 0.0);
        std::vector<double> tcp_position = std::vector<double>(3, 0.0);
        std::vector<double> tcp_orientation = std::vector<double>(3, 0.0);
    };


    /*setter*/
    void setForceValue(const std::vector<double>& force)
    {
        for (int i = 0; i < 3; ++i)
        {
            fts_.force[i] = force[i];
        }
    }

    void setTcpPosition(const std::vector<double>& tcp_position)
    {
        for (int i = 0; i < 3; ++i)
        {
            rs_.tcp_position[i] = tcp_position[i];
        }
    }

private:
    RobotContext& ctx_;
    bt::Blackboard& bb_;
    ForceTorqueSensor fts_;
    RobotState rs_;
};

} // namespace ca
