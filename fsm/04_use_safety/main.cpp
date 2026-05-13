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
class Normal : public fsm::State {
public:
    void onEnter() override  { }
    void onUpdate() override {
        // human monitoring
        // torque monitoring
     }
    void onExit() override   { }
    std::string name() const override { return "Normal"; }

private:
};

class ReducedSpeed : public fsm::State {
public:
    void onEnter() override  { }
    void onUpdate() override {
        // human monitoring
        // torque monitoring
     }
    void onExit() override   { }
    std::string name() const override { return "ReducedSpeed"; }

private:
};

class ProtectiveStop : public fsm::State {
public:
    void onEnter() override  { }
    void onUpdate() override { }
    void onExit() override   { }
    std::string name() const override { return "ProtectiveStop"; }

private:
};

class EmergencyStop : public fsm::State {
public:
    void onEnter() override  { }
    void onUpdate() override {
        // estop release -> normal
     }
    void onExit() override   { }
    std::string name() const override { return "EmergencyStop"; }

private:
};

} // namespace

int main() {
    fsm::StateMachine sm;

    // TODO: 4개 상태를 등록한다.
    sm.addState(std::make_unique<Normal>());
    sm.addState(std::make_unique<ReducedSpeed>());
    sm.addState(std::make_unique<ProtectiveStop>());
    sm.addState(std::make_unique<EmergencyStop>());

    // TODO: setOnTransition 으로 [T] from -> to (event) 로그 콜백을 단다.
    sm.setOnTransition([](const std::string& from,
                          const std::string& to,
                          const std::string& event) {});

    // 입력 시뮬레이션 변수 — 가드에서 캡처해 사용
    bool estop_pressed = false;
    (void)estop_pressed;

    // TODO: 전이 맵 등록 (README 의 표 참고)
    //   - Normal <-> ReducedSpeed                     (human_in_zone, zone_clear)
    //   - {Normal, ReducedSpeed} -> ProtectiveStop    (collision_detected)
    //   - ProtectiveStop -> Normal                    (reset)
    //   - {Normal, ReducedSpeed, ProtectiveStop} -> EmergencyStop  (estop_pressed)
    //   - EmergencyStop -> Normal                     (reset, guard: !estop_pressed)
    sm.addTransition("Normal", "ReducedSpeed", "human_in_zone");
    sm.addTransition("ReducedSpeed", "Normal", "zone_clear");
    
    for (const char* from : {"Normal", "ReducedSpeed"}) {
    sm.addTransition(from, "ProtectiveStop", "collision_detected");}
    
    for (const char* from : {"Normal", "ReducedSpeed", "ProtectiveStop"}) {
    sm.addTransition(from, "EmergencyStop", "estop_pressed");}
    
    sm.addTransition("ProtectiveStop", "Normal", "reset");
    sm.addTransition("EmergencyStop", "Normal", "reset", [&]{ return !estop_pressed; });


    /* run scenario */
    estop_pressed = false;
    sm.setInitial("Normal");
    
    std::cout << "\n>> human enters safety zone" << std::endl;
    sm.handleEvent("human_in_zone");

    std::cout << "\n>> human leaves safety zone" << std::endl;
    sm.handleEvent("zone_clear");

    std::cout << "\n>> collision detected" << std::endl;
    sm.handleEvent("collision_detected");

    std::cout << "\n>> operator presses reset" << std::endl;
    sm.handleEvent("reset");

    std::cout << "\n>> human enters safety zone" << std::endl;
    sm.handleEvent("human_in_zone");

    std::cout << "\n>> collision detected" << std::endl;
    sm.handleEvent("collision_detected");

    std::cout << "\n>> e-stop pressed" << std::endl;
    estop_pressed = true;
    sm.handleEvent("estop_pressed");

    std::cout << "\n>> operator presses reset (e-stop still latched)" << std::endl;
    if (!sm.handleEvent("reset")) {
        std::cout << "event 'reset' rejected: e-stop still latched\n";
    }

    std::cout << "\n>> e-stop released, reset again" << std::endl;
    estop_pressed = false;
    sm.handleEvent("reset");

    std::cout << "final state = " << sm.currentName() << "\n";
    return 0;
}
