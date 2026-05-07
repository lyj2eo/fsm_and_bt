// 05_use_assembly - 힘제어 기반 peg-in-hole 조립 통합 시나리오.
//
// btlib 에는 손대지 않는다. 트리 구성과 액션 본체는 모두 학습자가 작성한다.
// 자세한 트리 구조와 동작은 README.md 참고.

#include "bt/Sequence.h"
#include "bt/Action.h"
#include "bt/Retry.h"
#include "bt/Blackboard.h"

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

    // 시뮬레이션 상태 — 람다에서 캡처해 사용
    double pose_z          = 0.10;
    double force_z         = 0.0;
    double force_xy        = 0.0;
    int    insert_attempt  = 0;
    bool   gripper_closed  = false;

    (void)pose_z; (void)force_z; (void)force_xy;
    (void)insert_attempt; (void)gripper_closed;

    bt::Sequence root;

    // TODO: README 의 트리 구조에 맞춰 액션들을 root 에 추가한다.
    //
    //   - Pick 단계는 또 하나의 Sequence 로 묶어 root 의 자식으로 넣는다.
    //   - InsertAttempt 는 Retry(3) 로 감싼다.
    //   - DescendUntilContact 가 끝나면 bb->set("surface_z", pose_z) 처럼
    //     Blackboard 에 값을 저장한다. VerifyDepth 에서 그 값을 다시 읽어 비교한다.

    auto status = root.tick();
    std::cout << "result = " << toStr(status) << "\n";
    return 0;
}
