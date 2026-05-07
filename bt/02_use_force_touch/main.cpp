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

} // namespace

int main() {
    // 시뮬레이션 상태 — 람다에서 캡처해서 사용한다.
    double force_z = 0.0;     // 측정 힘 [N]
    double pose_z  = 0.10;    // 현재 TCP 높이 [m]
    bool   gripper_closed = false;
    const double contact_threshold = 5.0;  // [N]

    (void)force_z; (void)pose_z; (void)gripper_closed; // TODO 채우기 전 경고 방지용

    bt::Sequence root;

    // TODO: 아래 액션들을 차례로 root 에 add 한다.
    //
    //   1) EnableForceMode
    //   2) MoveAbovePart           (pose_z 를 안전 높이로 설정)
    //   3) DescendUntilContact     (천천히 pose_z 를 낮추며 force_z 가 contact_threshold 이상이 되면 Success)
    //   4) CheckContact            (force_z >= contact_threshold 면 Success, 아니면 Failure)
    //   5) CloseGripper
    //   6) Lift                    (다시 안전 높이로)
    //   7) DisableForceMode
    //
    // 각 액션은 std::make_unique<bt::Action>("Name", []{ ... return bt::Status::Success; })
    // 형태로 만들고, root.addChild(std::move(act)) 로 등록한다.

    auto status = root.tick();
    std::cout << "result = " << toStr(status) << "\n";
    return 0;
}
