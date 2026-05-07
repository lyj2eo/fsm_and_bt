#include "fsm/StateMachine.h"

namespace fsm {

StateMachine::StateMachine() = default;
StateMachine::~StateMachine() = default;

void StateMachine::addState(std::unique_ptr<State> /*state*/) {
    // TODO: states_ 에 등록한다. 키는 state->name().
}

void StateMachine::setInitial(const std::string& /*name*/) {
    // TODO: states_ 에서 해당 상태를 찾아 current_ 로 지정하고 onEnter() 호출.
}

void StateMachine::transitionTo(const std::string& /*name*/) {
    // TODO: current_->onExit() -> 새 상태->onEnter() 순서.
    //       동일 상태로의 전이는 어떻게 처리할지 결정한다.
}

void StateMachine::update() {
    // TODO: current_ 가 유효하면 onUpdate() 호출.
}

std::string StateMachine::currentName() const {
    // TODO: 현재 상태의 이름을 돌려준다. 없으면 "".
    return {};
}

} // namespace fsm
