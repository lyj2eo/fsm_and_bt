#pragma once

#include "bt/Action.h"
#include "../context/contact_context.h"

#include <cstddef>
#include <iostream>
#include <vector>

namespace contact {

class SetTaskSuccess : public bt::Action {
public:
    SetTaskSuccess(bt::Blackboard& bb)
        : bt::Action("SetTaskSuccess"),
          bb_(bb) {}

    bt::Status tick() override
    {
        std::cout << ">> task success\n";
        std::string task_status = "success";
        bb_.update("task_status", task_status);
        return bt::Status::Success;
    }

private:
    bt::Blackboard& bb_;
};

class SetTaskFail : public bt::Action {
public:
    SetTaskFail(bt::Blackboard& bb)
        : bt::Action("SetTaskFail"),
          bb_(bb) {}

    bt::Status tick() override
    {
        std::cout << ">> task fail\n";
        std::string task_status = "failure";
        bb_.update("task_status", task_status);
        return bt::Status::Success;
    }

private:
    bt::Blackboard& bb_;
};

class MoveUntilContact : public bt::Action {
public:
    explicit MoveUntilContact(ca::TaskInput& task_input, bt::Blackboard& bb)
        : bt::Action("MoveUntilContact"),
          task_input_(task_input),
          bb_(bb) {}

    bt::Status tick() override
    {
        std::cout << ">> move until contact\n";
        
        static int cnt = 0;
        cnt++;
        if (cnt > task_input_.time_out_cnt)
        {
            std::cout<<"[Failure] time out"<<std::endl;
            return bt::Status::Failure;
        }

        auto curr_position = bb_.get<double>("position");
        auto target_position = curr_position;

        if(bb_.get<bool>("is_contact"))
        {
            return bt::Status::Success;
        }

        target_position -= 0.02;
        bb_.update("target_position", target_position);

        return bt::Status::Running;
    }

private:
    ca::TaskInput& task_input_;
    bt::Blackboard& bb_;
};

class Pressing : public bt::Action {
public:
    explicit Pressing(ca::TaskInput& task_input, bt::Blackboard& bb)
        : bt::Action("Pressing"),
          task_input_(task_input),
          bb_(bb) {}

    bt::Status tick() override
    {
        static int cnt, pressing_cnt = 0;
        cnt++;
        std::cout << ">> pressing\n";

        auto cur_force = bb_.get<double>("force");

        double target_force = 0.0;
        bb_.update("target_force",target_force);

        if (cnt > task_input_.time_out_cnt)
        {
            std::cout<<"[Failure] time out"<<std::endl;
            return bt::Status::Failure;
        }

        if ( std::abs(task_input_.pressing_force - cur_force) <= task_input_.force_tolerance ) 
        {
            pressing_cnt ++; 
            if (pressing_cnt == task_input_.pressing_time_cnt)
            {
                return bt::Status::Success;
            }
        }

        return bt::Status::Running;
    }

private:
    // ca::RobotContext& ctx_;
    bt::Blackboard& bb_;
    ca::TaskInput& task_input_;
};


class MoveBackward : public bt::Action {
public:
    explicit MoveBackward(ca::RobotContext& ctx, bt::Blackboard& bb)
        : bt::Action("MoveBackward"),
          ctx_(ctx),
          bb_(bb) {}

    bt::Status tick() override
    {
        std::cout << ">> move backward\n";

        double curr_position = bb_.get<double>("position");
        bb_.update("target_position", curr_position + 0.1);
        
        std::cout << "   : move "<< curr_position << " --> " << curr_position + 0.1 << "\n";

        return bt::Status::Success;
    }

private:
    ca::RobotContext& ctx_;
    bt::Blackboard& bb_;
};


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
