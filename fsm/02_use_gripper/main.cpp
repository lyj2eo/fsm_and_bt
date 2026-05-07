// 02_use_gripper - fsmlib 을 사용해 그리퍼 FSM 을 만든다.
//
// 4개의 상태(Open, Closing, Closed, Opening)를 정의하고, main 에서 시나리오를
// 실행하면서 전이가 의도대로 일어나는지 출력으로 확인한다.
//
// 자세한 요구사항은 README.md 참고.

#include "fsm/StateMachine.h"

#include <iostream>
#include <memory>

namespace {

// ──────────────────────────────────────────────────────────────────────────
// TODO: 4개의 상태 클래스를 정의한다.
//
//   - GripperOpen    : 정지, "열린" 상태
//   - GripperClosing : 닫는 중. 일정 틱 후 GripperClosed 로 자동 전이
//   - GripperClosed  : 정지, "닫힌" 상태
//   - GripperOpening : 여는 중. 일정 틱 후 GripperOpen 으로 자동 전이
//
// 자기가 다른 상태로 전이하려면 StateMachine 에 접근할 수 있어야 한다.
// (생성자 인자로 StateMachine* 받기를 추천)
//
// 예시 시그니처:
//
//   class GripperClosing : public fsm::State {
//   public:
//       explicit GripperClosing(fsm::StateMachine* sm) : sm_(sm) {}
//       void onEnter() override;
//       void onUpdate() override;
//       void onExit() override;
//       std::string name() const override { return "Closing"; }
//   private:
//       fsm::StateMachine* sm_;
//       int ticks_ = 0;
//   };
// ──────────────────────────────────────────────────────────────────────────

} // namespace

int main() {
    fsm::StateMachine sm;

    // TODO: 4개 상태를 sm 에 등록한다.
    // TODO: 초기 상태를 "Open" 으로 설정한다.

    // 시나리오:
    //  1) close 명령 — Open 에서 Closing 으로 전이
    //  2) 몇 번 update — Closing 이 진행되고, 끝나면 Closed 로 자동 전이
    //  3) open 명령 — Closed 에서 Opening 으로 전이
    //  4) 몇 번 update — Opening 이 끝나면 Open 으로 자동 전이

    std::cout << "--- close command ---\n";
    sm.transitionTo("Closing");
    for (int i = 0; i < 5; ++i) sm.update();

    std::cout << "--- open command ---\n";
    sm.transitionTo("Opening");
    for (int i = 0; i < 5; ++i) sm.update();

    std::cout << "final = " << sm.currentName() << "\n";
    return 0;
}
