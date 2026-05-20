// 02_use_force_touch - 힘제어 기반 부드러운 접촉 픽업.
//
// btlib 는 수정하지 않는다. main 에서 액션 람다를 작성하고 Sequence 에 묶기만 하면 된다.
// 자세한 시나리오는 README.md 참고.

#include "bt/Sequence.h"
#include "bt/Action.h"

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

std::unique_ptr<bt::Action> moveTo(const std::string& name,
                                    double& pose_z,
                                    double target_z)
{
    return std::make_unique<bt::Action>( name, [&pose_z, target_z]() {
        std::cout << ">> move to pose : " << pose_z << " -> " << target_z << std::endl;
        pose_z = target_z;
        return bt::Status::Success;
    }
    );
}

} // namespace

int main() {
    double force_z = 0.0;
    double pose_z  = 0.0;
    double safety_height = 0.10;

    bool is_contact = false;
    const double contact_threshold = 5.0;

    bt::Sequence root("root",bt::Sequence::SequenceType::Resume);

    /* * * * add actions for sequence * * * */
    //   1) EnableForceMode
    auto enableForceMode = std::make_unique<bt::Action>("EnableForceMode", [] {
        std::cout << ">> force control on!"<< std::endl;
        return bt::Status::Success;
    });

    //   2) MoveWaypoint
    auto moveWaypoint = std::make_unique<bt::Action>("MoveWaypoint", [&pose_z, safety_height] {
        pose_z = safety_height;
        std::cout << ">> safety height settings, h = " << pose_z << std::endl;
        return bt::Status::Success;
    });

    //   3) MoveUntilContact
    auto moveUntilContact = std::make_unique<bt::Action>("MoveUntilContact", [&] {
        pose_z -= 0.02;
        force_z = (pose_z <= 0.05) ? 6.0 : 0.0; 
        std::cout << ">> current pose_z = " << pose_z << std::endl;

        if (force_z >= contact_threshold){
            is_contact = true;
            std::cout << ">> contact detected!" << std::endl;
            return bt::Status::Success;
        }
        return bt::Status::Running;
    });   
    
    //   4) CheckContact
    auto checkContact = std::make_unique<bt::Action>("CheckContact", [&is_contact, &force_z, contact_threshold] {
        if (is_contact && force_z >= contact_threshold)
            return bt::Status::Success;

        std::cout << ">> contact is not detected!" << std::endl;
        return bt::Status::Failure;
    });

    //   5) Backward
    auto backward = std::make_unique<bt::Action>("Backward", [&pose_z] {
        pose_z += 0.01;
        std::cout << ">> move backward slightly" << std::endl;
        return bt::Status::Success;
    });

    //   6) DisableForceMode
    auto disableForceMode = std::make_unique<bt::Action>("DisableForceMode", [] {
        std::cout << ">> force control off!"<< std::endl;
        return bt::Status::Success;
    });

    //   7) Move
    auto move = std::make_unique<bt::Action>("Move", [&pose_z, safety_height] {
        pose_z = safety_height;
        std::cout << ">> move to safety height" << std::endl;
        return bt::Status::Success;
    });



    /* * * * add children to sequence * * * */
    root.addChild(std::move(enableForceMode));
    root.addChild(moveTo("MoveWaypoint", pose_z, safety_height));
    root.addChild(std::move(moveUntilContact));
    root.addChild(std::move(checkContact));
    root.addChild(std::move(backward));
    root.addChild(std::move(disableForceMode));
    root.addChild(moveTo("Move", pose_z, safety_height));

    

    /* * * * run scenario * * * */
    bt::Status status;
    do {
        status = root.tick();
    } while (status == bt::Status::Running);

    std::cout << "result = " << toStr(status) << "\n";
    return 0;
}
