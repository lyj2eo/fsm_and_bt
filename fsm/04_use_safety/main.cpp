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
    void onUpdate() override {}
    void onExit() override   { }
    std::string name() const override { return "EmergencyStop"; }

private:
};

} // namespace

int main() {
    fsm::StateMachine sm;

    // set states
    sm.addState(std::make_unique<Normal>());
    sm.addState(std::make_unique<ReducedSpeed>());
    sm.addState(std::make_unique<ProtectiveStop>());
    sm.addState(std::make_unique<EmergencyStop>());

    sm.setOnTransition([](const std::string& from,
                          const std::string& to,
                          const std::string& event) {});

    // emergency stop
    bool estop_pressed = false;

    // set transitions
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
