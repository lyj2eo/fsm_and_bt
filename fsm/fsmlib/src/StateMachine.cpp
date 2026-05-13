#include "fsm/StateMachine.h"
#include <iostream>

namespace fsm {

StateMachine::StateMachine() = default;
StateMachine::~StateMachine() = default;

void StateMachine::addState(std::unique_ptr<State> state)
{
    states_[state->name()] = std::move(state);
}

void StateMachine::setInitial(const std::string& name)
{
    auto it = states_.find(name);

    if (it == states_.end()) {
        std::cerr << "State '" << name << "' is not found.\n";
        return;
    }
    current_ = it->second.get();
    current_->onEnter();
}

void StateMachine::transitionTo(const std::string& name)
{
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
            std::cout<<"[Transition] '"<<current_->name()<<"' -> '"<<name<<"'"<<std::endl;
            current_->onExit(); 
            setInitial(name);
        }
    } else{
        std::cerr << "current state is null.\n";
    }
}

void StateMachine::update() 
{
    if(!current_) {
        std::cerr << "No current state. update is ignored.\n";
        return;
    }
    current_->onUpdate();
}

std::string StateMachine::currentName() const 
{
    return {current_ ? current_->name() : ""};
}

std::string StateMachine::makeKey(const std::string& from, const std::string& event) 
{
    return from + "+" + event;
}

void StateMachine::addTransition(const std::string& from,
                                 const std::string& to,
                                 const std::string& event,
                                 Guard guard) {

    auto key = makeKey(from, event);
    transitions_[key].push_back({ from, to, event, std::move(guard) }); 
    std::cout << "[add Transition] " << from << " -> " << to << " (" << event << ")\n";
}

bool StateMachine::handleEvent(const std::string& event) 
{

    if (!current_) {
        std::cerr << "No current state.\n";
        return false;
    }
    
    auto key = makeKey(current_->name(), event);
    auto it = transitions_.find(key);
    if (it == transitions_.end()) {
        std::cerr << "No transition found for event '" << event << "' in state '" << current_->name() << "'.\n";
        return false;
    }

    // guard check
    for (const Transition& tr : it->second) {
        if (tr.guard && !tr.guard()) 
        continue;
        
        std::string from = current_->name();
        const auto& to = tr.to;
        
        transitionTo(to);
        update();
        if (onTransition_) onTransition_(from, to, event);

        return true; // first matching transition
    }

    std::cerr << "[Guard] failed"<< std::endl;
    return false;
}

void StateMachine::setOnTransition(TransitionCallback cb) {
    onTransition_ = std::move(cb);
}

} // namespace fsm
