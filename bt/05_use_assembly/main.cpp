#include "bt/Sequence.h"
#include "bt/Action.h"
#include "bt/Retry.h"
#include "bt/Blackboard.h"
#include "actions/assembly_actions.h"

#include <iostream>
#include <memory>

namespace {

const char* toStr(bt::Status s) {
    switch (s) {
        case bt::Status::Success: return "Success";
        case bt::Status::Failure: return "Failure";
        case bt::Status::Running: return "Running";
    }
    return "?";
}

} // namespace

int main() {
    bt::Blackboard bb;

    double pose_z          = 0.0;               //bb - state
    double force_z         = 0.0;               //bb - sensor
    double force_xy        = 0.0;               //bb - sensor
    double current_depth   = 0.03;              //bb - state
    bool   gripper_closed  = false;             //bb - task.gripper
    
    int max_attempt_cnt = 5;                    //bb - parameter

    const double waypoint_part_z = 0.10;        //api
    const double waypoint_lift_z = 0.12;        //api
    const double waypoint_hole_z = 0.10;        //api

    const double contact_force_threshold = 5.0; //bb - parameter
    const double align_force_threshold = 2.0;   //bb - parameter

    const double target_depth = 0.03;           //bb - target
    const double depth_tolerance = 0.005;       //bb - parameter

    bb.set("pose_z", pose_z);
    bb.set("surface_z", pose_z);
    bb.set("force_z", force_z);
    bb.set("force_xy", force_xy);
    bb.set("current_depth", current_depth);
    bb.set("gripper_closed", gripper_closed);
    bb.set("max_attempt_cnt", max_attempt_cnt);
    bb.set("waypoint_part_z", waypoint_part_z);
    bb.set("waypoint_lift_z", waypoint_lift_z);
    bb.set("waypoint_hole_z", waypoint_hole_z);
    bb.set("contact_force_threshold", contact_force_threshold);
    bb.set("align_force_threshold", align_force_threshold);
    bb.set("target_depth", target_depth);
    bb.set("depth_tolerance", depth_tolerance);


    bt::Sequence root("root", bt::Sequence::SequenceType::Resume);

    root.setBlackboard(bb);


    /* * * * create "pick sequence" actions * * * */
    auto pick = std::make_unique<bt::Sequence>("pick", bt::Sequence::SequenceType::Resume);
    pick->addChild(std::make_unique<assembly::MoveAbovePart>(pose_z, waypoint_part_z));
    pick->addChild(std::make_unique<assembly::DescendUntilContact>(pose_z, force_z, contact_force_threshold));
    pick->addChild(std::make_unique<assembly::CloseGripper>(gripper_closed));
    pick->addChild(std::make_unique<assembly::Lift>(pose_z, waypoint_lift_z));


    /* * * * create "retry decorator" actions * * * */
    auto retry = std::make_unique<bt::Retry>("insert retry",max_attempt_cnt);
    retry->addChild(std::make_unique<assembly::InsertAttempt>(force_xy, align_force_threshold));
    

    /* * * * create "root sequence" actions * * * */
    root.addChild(std::make_unique<assembly::EnableForceMode>());
    root.addChild(std::move(pick));
    root.addChild(std::make_unique<assembly::MoveAboveHole>(pose_z, waypoint_hole_z));
    root.addChild(std::move(retry));
    root.addChild(std::make_unique<assembly::VerifyDepth>(current_depth, target_depth, depth_tolerance));
    root.addChild(std::make_unique<assembly::ReleaseGripper>(gripper_closed));
    root.addChild(std::make_unique<assembly::DisableForceMode>());


    /* run */
    bt::Status status;
    do {
        status = root.tick();
    } while (status == bt::Status::Running);

    std::cout << "result = " << toStr(status) << "\n";
    return 0;
}