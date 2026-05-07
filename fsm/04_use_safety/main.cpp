// 04_use_safety - 협동로봇 안전상태 FSM.
//
// fsmlib 의 addTransition / handleEvent / setOnTransition 을 모두 활용한다.
// 라이브러리에는 손대지 않는다.
//
// 자세한 시나리오와 전이 맵은 README.md 참고.

#include "fsm/StateMachine.h"

#include <iostream>
#include <memory>

namespace {

// TODO: 4개의 안전 상태 클래스를 정의한다.
//   - Normal
//   - ReducedSpeed
//   - ProtectiveStop
//   - EmergencyStop
//
// 각 상태의 onEnter / onExit 에서 출력할 내용은 자유.
// 한 클래스로 묶어서 이름만 바꿔주는 LoggingState 패턴을 써도 된다 (03번 참고).

} // namespace

int main() {
    fsm::StateMachine sm;

    // TODO: 4개 상태를 등록한다.
    // TODO: setOnTransition 으로 [T] from -> to (event) 로그 콜백을 단다.

    // 입력 시뮬레이션 변수 — 가드에서 캡처해 사용
    bool estop_pressed = false;
    (void)estop_pressed;

    // TODO: 전이 맵 등록 (README 의 표 참고)
    //   - Normal <-> ReducedSpeed                     (human_in_zone, zone_clear)
    //   - {Normal, ReducedSpeed} -> ProtectiveStop    (collision_detected)
    //   - ProtectiveStop -> Normal                    (reset)
    //   - {Normal, ReducedSpeed, ProtectiveStop} -> EmergencyStop  (estop_pressed)
    //   - EmergencyStop -> Normal                     (reset, guard: !estop_pressed)

    // TODO: 초기 상태를 "Normal" 로.

    auto step = [](const char* msg) { std::cout << "\n>> " << msg << "\n"; };

    step("human enters safety zone");
    sm.handleEvent("human_in_zone");

    step("human leaves safety zone");
    sm.handleEvent("zone_clear");

    step("collision detected");
    sm.handleEvent("collision_detected");

    step("operator presses reset");
    sm.handleEvent("reset");

    step("e-stop pressed");
    estop_pressed = true;
    sm.handleEvent("estop_pressed");

    step("operator presses reset (e-stop still latched)");
    if (!sm.handleEvent("reset")) {
        std::cout << "event 'reset' rejected: e-stop still latched\n";
    }

    step("e-stop released, reset again");
    estop_pressed = false;
    sm.handleEvent("reset");

    std::cout << "\nfinal = " << sm.currentName() << "\n";
    return 0;
}
