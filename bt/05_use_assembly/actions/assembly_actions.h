#pragma once

#include "bt/Action.h"

#include <cmath>
#include <iostream>

namespace assembly {

class EnableForceMode : public bt::Action {
public:
    EnableForceMode()
        : bt::Action("EnableForceMode")
    {
    }

    bt::Status tick() override {
        std::cout << ">> force control on!\n";
        return bt::Status::Success;
    }
};

class DisableForceMode : public bt::Action {
public:
    DisableForceMode()
        : bt::Action("DisableForceMode")
    {
    }

    bt::Status tick() override {
        std::cout << ">> force control off!\n";
        return bt::Status::Success;
    }
};

class MoveAbovePart : public bt::Action {
public:
    MoveAbovePart(double& pose_z, double target_z)
        : bt::Action("MoveAbovePart"),
          pose_z_(pose_z),
          target_z_(target_z)
    {
    }

    bt::Status tick() override {
        std::cout << ">> move above part: "
                  << pose_z_ << " -> " << target_z_ << "\n";

        pose_z_ = target_z_;
        return bt::Status::Success;
    }

private:
    double& pose_z_;
    double target_z_;
};

class DescendUntilContact : public bt::Action {
public:
    DescendUntilContact(double& pose_z,
                        double& force_z,
                        double contact_threshold)
        : bt::Action("DescendUntilContact"),
          pose_z_(pose_z),
          force_z_(force_z),
          contact_threshold_(contact_threshold)
    {
    }

    bt::Status tick() override {
        pose_z_ -= 0.02;
        force_z_ = (pose_z_ <= 0.05) ? 6.0 : 0.0;

        std::cout << ">> descending, pose_z = " << pose_z_
                  << ", Fz = " << force_z_ << "\n";

        if (force_z_ >= contact_threshold_) {
            if (!bb_) {
                std::cout << ">> blackboard is null\n";
                return bt::Status::Failure;
            }

            bb_->update("surface_z", pose_z_);
 
            std::cout << ">> contact detected, surface_z = "
                      << pose_z_ << "\n";

            return bt::Status::Success;
        }

        return bt::Status::Running;
    }

private:
    double& pose_z_;
    double& force_z_;
    double contact_threshold_;
};

class CloseGripper : public bt::Action {
public:
    explicit CloseGripper(bool& gripper_closed)
        : bt::Action("CloseGripper"),
          gripper_closed_(gripper_closed)
    {
    }

    bt::Status tick() override {
        gripper_closed_ = true;
        std::cout << ">> close gripper\n";
        return bt::Status::Success;
    }

private:
    bool& gripper_closed_;
};

class Lift : public bt::Action {
public:
    Lift(double& pose_z, double lift_z)
        : bt::Action("Lift"),
          pose_z_(pose_z),
          lift_z_(lift_z)
    {
    }

    bt::Status tick() override {
        std::cout << ">> lift: "
                  << pose_z_ << " -> " << lift_z_ << "\n";

        pose_z_ = lift_z_;
        return bt::Status::Success;
    }

private:
    double& pose_z_;
    double lift_z_;
};

class MoveAboveHole : public bt::Action {
public:
    MoveAboveHole(double& pose_z, double target_z)
        : bt::Action("MoveAboveHole"),
          pose_z_(pose_z),
          target_z_(target_z)
    {
    }

    bt::Status tick() override {
        std::cout << ">> move above hole, z: "
                  << pose_z_ << " -> " << target_z_ << "\n";

        pose_z_ = target_z_;
        return bt::Status::Success;
    }

private:
    double& pose_z_;
    double target_z_;
};

class InsertAttempt : public bt::Action {
public:
    InsertAttempt(double& force_xy,
                  double align_force_threshold)
        : bt::Action("InsertAttempt"),
          force_xy_(force_xy),
          align_force_threshold_(align_force_threshold)
    {
    }

    bt::Status tick() override {
        if (!bb_) {
            std::cout << ">> blackboard is null\n";
            return bt::Status::Failure;
        }
        ++attempt_count_;

        force_xy_ = 10.0 - 4.0 * (attempt_count_ - 1);

        bb_->update("force_xy", force_xy_);
        std::cout << ">> [InsertAttempt #" << attempt_count_
                  << "] Fxy = " << force_xy_;

        if (force_xy_ > align_force_threshold_) {
            std::cout << " -> too high, FAIL\n";
            return bt::Status::Failure;
        }

        std::cout << " -> inserted\n";
        attempt_count_ = 0;
        return bt::Status::Success;
    }

private:
    double& force_xy_;
    double align_force_threshold_;
    int attempt_count_ = 0;
};

class VerifyDepth : public bt::Action {
public:
    VerifyDepth(double& current_depth,
                double target_depth,
                double tolerance)
        : bt::Action("VerifyDepth"),
          current_depth_(current_depth),
          target_depth_(target_depth),
          tolerance_(tolerance)
    {
    }

    bt::Status tick() override {
        if (!bb_) {
            std::cout << ">> blackboard is null\n";
            return bt::Status::Failure;
        }

        if (!bb_->has("surface_z")) {
            std::cout << ">> surface_z is missing\n";
            return bt::Status::Failure;
        }

        const double surface_z = bb_->get<double>("surface_z");
        const double depth_error = std::abs(current_depth_ - target_depth_);

        std::cout << ">> verify depth"
                  << ", surface_z = " << surface_z
                  << ", current_depth = " << current_depth_
                  << ", target_depth = " << target_depth_
                  << ", error = " << depth_error << "\n";

        if (depth_error <= tolerance_) {
            return bt::Status::Success;
        }

        return bt::Status::Failure;
    }

private:
    double& current_depth_;
    double target_depth_;
    double tolerance_;
};

class ReleaseGripper : public bt::Action {
public:
    explicit ReleaseGripper(bool& gripper_closed)
        : bt::Action("ReleaseGripper"),
          gripper_closed_(gripper_closed)
    {
    }

    bt::Status tick() override {
        gripper_closed_ = false;
        std::cout << ">> release gripper\n";
        return bt::Status::Success;
    }

private:
    bool& gripper_closed_;
};

} // namespace assembly