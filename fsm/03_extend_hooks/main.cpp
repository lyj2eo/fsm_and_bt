// 03_extend_hooks - 라이브러리에 추가한 전이 맵(transition map) 과 콜백을 활용한다.
//
// 학습자가 fsmlib 에 addTransition / handleEvent / setOnTransition 을 추가했다는
// 전제로 작성되어 있다. 라이브러리를 먼저 확장하지 않으면 컴파일이 실패한다.
//
// 자세한 요구사항은 README.md 참고.

#include "fsm/StateMachine.h"

#include <iostream>
#include <memory>

namespace {

// 단순한 로그용 상태. 4종류 모두 비슷하므로 한 클래스로 묶었다.
class LoggingState : public fsm::State {
public:
    explicit LoggingState(std::string n) : name_(std::move(n)) {}
    void onEnter() override  { std::cout << "  enter  " << name_ << "\n"; }
    void onUpdate() override {}
    void onExit() override   { std::cout << "  exit   " << name_ << "\n"; }
    std::string name() const override { return name_; }
private:
    std::string name_;
};

} // namespace

int main() {
    fsm::StateMachine sm;
    sm.addState(std::make_unique<LoggingState>("Idle"));
    sm.addState(std::make_unique<LoggingState>("Initializing"));
    sm.addState(std::make_unique<LoggingState>("Ready"));
    sm.addState(std::make_unique<LoggingState>("Executing"));

    bool calibrated = false;

    // 전이 맵 등록 (README 의 표 참고).
    sm.addTransition("Idle",         "Initializing", "start");
    sm.addTransition("Initializing", "Ready",        "tick", [&]{ return calibrated; });
    sm.addTransition("Ready",        "Executing",    "run");
    sm.addTransition("Executing",    "Ready",        "done");

    sm.setOnTransition([](const std::string& f,
                          const std::string& t,
                          const std::string& e) {
        std::cout << "[T] " << f << " -> " << t << " (" << e << ")\n";
    });

    sm.setInitial("Idle");

    sm.handleEvent("start");

    // 캘리브레이션 전 — 가드에 막혀 발화 안 됨.
    if (!sm.handleEvent("tick")) {
        std::cout << "event 'tick' rejected: not calibrated yet\n";
    }

    // 캘리브레이션 완료 후 다시 시도.
    calibrated = true;
    sm.handleEvent("tick");

    sm.handleEvent("run");
    sm.handleEvent("done");

    return 0;
}
