#pragma once

#include <vector>

namespace ca {

// inline constexpr const char* CONTACT_TASK = "contact_detection";
inline constexpr const char* CONTACT_TASK = "static_pressing";
inline constexpr const char* ROBOT_FRAME  = "tcp";

struct RobotContext {
    bool force_mode_enabled = false;
};

struct TaskInput {
    double pressing_force = 10.0;
    double force_tolerance = 0.5;

    int time_out_cnt = 10;
    int pressing_time_cnt = 5;
};

} // namespace ca
