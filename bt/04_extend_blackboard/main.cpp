// 04_extend_blackboard - 노드 간 데이터 공유.
//
// 학습자가 Blackboard 를 라이브러리에 추가했다는 전제. (Blackboard.h 가 없으면 빌드 실패)

#include "bt/Sequence.h"
#include "bt/Action.h"
#include "bt/Blackboard.h"   // <-- 학습자가 만들 헤더

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
    auto bb = std::make_shared<bt::Blackboard>();

    bt::Sequence root("root", bt::Sequence::SequenceType::Resume);

    const double fz = 7.2;
    const double target_fz = 5.0;
    double pose_z = 0.0;
    const double gain = 0.005;


    /* * * * add actions for sequence * * * */

    // 1) enable force mode
    auto enableForceMode = std::make_unique<bt::Action>("EnableForceMode", []{
        std::cout << ">> force control on!\n";
        return bt::Status::Success;
    });

    // 2) measure Fz
    auto measureFz = std::make_unique<bt::Action>("MeasureFz",[bb, fz] {
            const double value_fz = fz;
            bb->set("fts_fz", value_fz);

            return bt::Status::Success;
    });

    // 3) update Pose correction
    auto updatePoseCorrection = std::make_unique<bt::Action>("UpdatePoseCorrection",[bb, target_fz, gain] {
            if (!bb->has("fts_fz")) {
                std::cout << "[Blackboard Error] value is missing\n";
                return bt::Status::Failure;
            }

            const double fz = bb->get<double>("fts_fz");
            const double err = fz - target_fz;
            const double value_dz_correction = -err * gain;

            bb->set("dz_correction", value_dz_correction);

            return bt::Status::Success;
    });

    // 4) apply position
    auto applyPosition = std::make_unique<bt::Action>("ApplyPosition", [bb, &pose_z] {
            if (!bb->has("dz_correction")) {
                std::cout << "[Blackboard Error] value is missing\n";
                return bt::Status::Failure;
            }

            const double dz = bb->get<double>("dz_correction");
            pose_z += dz;

            return bt::Status::Success;
        }
    );

    // 5) disable force mode
    auto disableForceMode = std::make_unique<bt::Action>("DisableForceMode", []{
        std::cout << ">> force control off!\n";
        return bt::Status::Success;
    });



    /* * * * add children to sequence * * * */
    root.addChild(std::move(enableForceMode));
    root.addChild(std::move(measureFz));
    root.addChild(std::move(updatePoseCorrection));
    root.addChild(std::move(applyPosition));
    root.addChild(std::move(disableForceMode));



    /* * * * run scenario * * * */
    bt::Status status;
    do {
        status = root.tick();
    } while (status == bt::Status::Running);

    std::cout << "result = " << toStr(status) << "\n";
    return 0;
}
