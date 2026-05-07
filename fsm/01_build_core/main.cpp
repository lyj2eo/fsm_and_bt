// 01_build_core - fsmlib 의 StateMachine 이 제대로 동작하는지 확인하는 main.
//
// 이 파일은 수정하지 않는다. fsm/fsmlib 의 빈 함수들을 구현해서
// 아래 "기대 출력" 이 그대로 나오게 만들면 성공이다.
//
// 기대 출력:
//   [A] enter
//   [A] update
//   [A] exit
//   [B] enter
//   [B] update
//   current = B

#include "fsm/StateMachine.h"

#include <iostream>
#include <memory>

namespace {

class StateA : public fsm::State {
public:
    void onEnter() override  { std::cout << "[A] enter\n"; }
    void onUpdate() override { std::cout << "[A] update\n"; }
    void onExit() override   { std::cout << "[A] exit\n"; }
    std::string name() const override { return "A"; }
};

class StateB : public fsm::State {
public:
    void onEnter() override  { std::cout << "[B] enter\n"; }
    void onUpdate() override { std::cout << "[B] update\n"; }
    void onExit() override   { std::cout << "[B] exit\n"; }
    std::string name() const override { return "B"; }
};

} // namespace

int main() {
    fsm::StateMachine sm;
    sm.addState(std::make_unique<StateA>());
    sm.addState(std::make_unique<StateB>());

    sm.setInitial("A");
    sm.update();
    sm.transitionTo("B");
    sm.update();

    std::cout << "current = " << sm.currentName() << "\n";
    return 0;
}
