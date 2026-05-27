#pragma once

#include "bt/Action.h"
#include "../context/contact_context.h"

#include <cstddef>
#include <iostream>
#include <vector>

namespace contact {

class EnableForceMode : public bt::Action {
public:
    explicit EnableForceMode(ca::RobotContext& ctx)
        : bt::Action("EnableForceMode"),
          ctx_(ctx) {}

    bt::Status tick() override
    {
        ctx_.force_mode_enabled = true;
        std::cout << ">> force mode enabled\n";
        return bt::Status::Success;
    }

private:
    ca::RobotContext& ctx_;
};

class Wait : public bt::Action {
public:
    explicit Wait(ca::RobotContext& ctx, bt::Blackboard& bb)
        : bt::Action("Wait"),
          ctx_(ctx),
          bb_(bb) {}

    bt::Status tick() override
    {
        std::cout << ">> wait\n";
        if (bb_.get<bool>("is_contact")) {
            return bt::Status::Success;
        }
        return bt::Status::Running;
    }

private:
    ca::RobotContext& ctx_;
    bt::Blackboard& bb_;
};

class DisableForceMode : public bt::Action {
public:
    explicit DisableForceMode(ca::RobotContext& ctx)
        : bt::Action("DisableForceMode"),
          ctx_(ctx) {}

    bt::Status tick() override
    {
        ctx_.force_mode_enabled = false;
        std::cout << ">> force mode disabled\n";
        return bt::Status::Success;
    }

private:
    ca::RobotContext& ctx_;
};

} // namespace contact
