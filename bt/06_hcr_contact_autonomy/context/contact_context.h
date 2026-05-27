#pragma once

#include <vector>

namespace ca {

inline constexpr const char* CONTACT_TASK = "contact_detection";
inline constexpr const char* ROBOT_FRAME  = "tcp";

struct RobotContext {
    bool force_mode_enabled = false;
};

} // namespace ca
