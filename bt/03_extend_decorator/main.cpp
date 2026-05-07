// 03_extend_decorator - Retry 데코레이터로 peg-in-hole 삽입 재시도.
//
// 학습자가 라이브러리에 Decorator / Retry / Inverter 를 추가했다는 전제.
// 라이브러리 확장 전에는 Retry.h 를 찾을 수 없으므로 빌드 실패한다 — 정상이다.

#include "bt/Sequence.h"
#include "bt/Action.h"
#include "bt/Retry.h"      // <-- 학습자가 만들 헤더

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
    // 시뮬레이션 상태
    int    attempt   = 0;       // 현재 시도 회차
    double force_xy  = 0.0;     // 측면 힘
    const double align_thresh = 2.0;

    bt::Sequence root;

    root.addChild(std::make_unique<bt::Action>("EnableForceMode", []{
        std::cout << "[EnableForceMode] on\n";
        return bt::Status::Success;
    }));

    root.addChild(std::make_unique<bt::Action>("ApproachHole", []{
        std::cout << "[ApproachHole] over hole\n";
        return bt::Status::Success;
    }));

    // Retry(3) 안에 InsertAttempt 액션을 넣는다.
    auto retry = std::make_unique<bt::Retry>(3);
    retry->setChild(std::make_unique<bt::Action>("InsertAttempt", [&]{
        ++attempt;
        // 시도가 거듭될수록 정렬이 좋아져 force_xy 가 줄어든다고 가정
        force_xy = 8.4 / static_cast<double>(attempt) - 0.4 * (attempt - 1);
        std::cout << "[InsertAttempt #" << attempt << "] Fxy = " << force_xy;
        if (force_xy > align_thresh) {
            std::cout << "  -> too high, FAIL\n";
            return bt::Status::Failure;
        }
        std::cout << "  -> inserted\n";
        return bt::Status::Success;
    }));
    root.addChild(std::move(retry));

    root.addChild(std::make_unique<bt::Action>("VerifyDepth", []{
        std::cout << "[VerifyDepth] depth ok\n";
        return bt::Status::Success;
    }));

    root.addChild(std::make_unique<bt::Action>("DisableForceMode", []{
        std::cout << "[DisableForceMode] off\n";
        return bt::Status::Success;
    }));

    auto status = root.tick();
    std::cout << "result = " << toStr(status) << "\n";
    return 0;
}
