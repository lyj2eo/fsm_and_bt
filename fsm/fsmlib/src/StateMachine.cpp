#include "fsm/StateMachine.h"
#include <iostream>

namespace fsm {

StateMachine::StateMachine() = default;
StateMachine::~StateMachine() = default;

void StateMachine::addState(std::unique_ptr<State> state) {
    // TODO: states_ 에 등록한다. 키는 state->name().
    states_[state->name()] = std::move(state);
}

void StateMachine::setInitial(const std::string& name) {
    // TODO: states_ 에서 해당 상태를 찾아 current_ 로 지정하고 onEnter() 호출.
    auto it = states_.find(name);

    if (it == states_.end()) {
        std::cerr << "State '" << name << "' is not found.\n";
        return;
    }
    current_ = it->second.get();
    current_->onEnter();
}

void StateMachine::transitionTo(const std::string& name) {
    // TODO: current_->onExit() -> 새 상태->onEnter() 순서.
    //       동일 상태로의 전이는 어떻게 처리할지 결정한다.
    auto it = states_.find(name);
    if (it == states_.end()) {
        std::cerr << "[State transition failure] state '"
                  << name << "' is not found.\n";
        return;
    }

    if(current_){
        if(current_->name() == name){
            std::cout << "[State transition failure] state '" << name << "' is already the current state.\n";
            return;
        } else{
            std::cout<<"["<<current_->name()<<"]"<<" -> ["<<name<<"]"<<std::endl;
            current_->onExit(); 
            setInitial(name);
        }
    } else{
        std::cerr << "current state is null.\n";
    }
}

void StateMachine::update() {
    // TODO: current_ 가 유효하면 onUpdate() 호출.
    if(!current_) {
        std::cerr << "No current state. update is ignored.\n";
        return;
    }
    current_->onUpdate();
}

std::string StateMachine::currentName() const {
    // TODO: 현재 상태의 이름을 돌려준다. 없으면 "".
    return {current_ ? current_->name() : ""};
}

} // namespace fsm
