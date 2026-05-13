// 03_extend_hooks - 라이브러리에 추가한 전이 맵(transition map) 과 콜백을 활용한다.
//
// 학습자가 fsmlib 에 addTransition / handleEvent / setOnTransition 을 추가했다는
// 전제로 작성되어 있다. 라이브러리를 먼저 확장하지 않으면 컴파일이 실패한다.
//
// 자세한 요구사항은 README.md 참고.

#include "fsm/StateMachine.h"

#include <iostream>
#include <memory>

namespace fsm {

// class LoggingState : public fsm::State {
// public:
//     explicit LoggingState(std::string n) : name_(std::move(n)) {}
//     void onEnter() override  { std::cout << "  enter  " << name_ << "\n"; }
//     void onUpdate() override { std::cout << "  update " << name_ << "\n"; }
//     void onExit() override   { std::cout << "  exit   " << name_ << "\n"; }
//     std::string name() const override { return name_; }
// private:
//     std::string name_;
// };

class Idle : public fsm::State {
public:
    void onEnter() override  { }
    void onUpdate() override { std::cout << "current state is :" << name() << std::endl; }
    void onExit() override   { }
    std::string name() const override { return "Idle"; }

private:
};

class Initializing : public fsm::State {
public:
    explicit Initializing(bool& calibrated) : calibrated_(calibrated) {}

    void onEnter() override  { 
        tick_ = 0;
        calibrated_ = false;
        std::cout << "[Initializing] start calibration\n";
    }
    void onUpdate() override {
        
        if (tick_ >= calibration_tick) return;

        ++tick_;
        std::cout << "[Initializing] calibration in progress... "
                  << tick_ << "/" << calibration_tick
                  << " (" << tick_ * 100 / calibration_tick << "%)\n";
 
        if (tick_ >= calibration_tick) {
            calibrated_ = true;
            std::cout << "[Initializing] calibration complete\n";
        }
    }
    void onExit() override   { }
    std::string name() const override { return "Initializing"; }

private:
    int tick_ = 0;
    const int calibration_tick = 15;
    bool& calibrated_;
};

class Ready : public fsm::State {
public:
    void onEnter() override  { }
    void onUpdate() override {
        // E-Stop 모니터링
     }
    void onExit() override   { }
    std::string name() const override { return "Ready"; }

private:
};

class Executing : public fsm::State {
public:
    void onEnter() override  { }
    void onUpdate() override {
        // 작업 완료 여부 확인
        // E-Stop 모니터링
        // 외력감지 ...
    }
    void onExit() override   { }
    std::string name() const override { return "Executing"; }

private:
};

} // namespace fsm

int main() {
    fsm::StateMachine sm;
    
    bool calibrated = false;

    // set states
    sm.addState(std::make_unique<fsm::Idle>());
    sm.addState(std::make_unique<fsm::Initializing>(calibrated));
    sm.addState(std::make_unique<fsm::Ready>());
    sm.addState(std::make_unique<fsm::Executing>());

    // set transitions
    sm.addTransition("Idle",         "Initializing", "start");
    sm.addTransition("Initializing", "Ready",        "tick", [&]{ return calibrated; });
    sm.addTransition("Ready",        "Executing",    "run");
    sm.addTransition("Executing",    "Ready",        "done");

    sm.setOnTransition([](const std::string& f,
                          const std::string& t,
                          const std::string& e) {
    });

    /* run scenario */
    sm.setInitial("Idle");
    
    std::cout << ">> commnand start" << std::endl;
    sm.handleEvent("start");

    // before calibration
    std::cout << ">> commnand tick" << std::endl;
    sm.handleEvent("tick");

    // update
    for (int i = 0; i < 20; ++i){
        sm.update();
    }
    // if (!sm.handleEvent("tick")) {
    //     std::cout << "event 'tick' rejected: not calibrated yet\n";
    // }

    // after calibration
    std::cout << ">> commnand tick" << std::endl;
    sm.handleEvent("tick");

    std::cout << ">> commnand run" << std::endl;
    sm.handleEvent("run");

    std::cout << ">> commnand done" << std::endl;
    sm.handleEvent("done");
    

    return 0;
}
