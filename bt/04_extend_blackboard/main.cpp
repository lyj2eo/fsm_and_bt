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

    bt::Sequence root;

    // TODO: 다음 액션들을 root 에 추가한다. 람다에서 bb 를 캡처해 사용.
    //
    //   1) EnableForceMode
    //   2) MeasureFz          : 임의의 시뮬레이션 값(예: 7.2)을 bb->set("force_z", v) 로 저장
    //   3) DecideAdjustment   : 목표 5.0 N 과의 오차로 bb->set("dz_correction", -err * gain)
    //   4) ApplyAdjustment    : bb->get("dz_correction") 만큼 z 를 줄였다고 출력
    //   5) DisableForceMode

    auto status = root.tick();
    std::cout << "result = " << toStr(status) << "\n";
    return 0;
}
