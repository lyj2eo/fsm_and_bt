#pragma once

#include "State.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace fsm {

// 상태들을 보관하고, 현재 상태의 라이프사이클을 호출하는 단순한 FSM.
class StateMachine {
public:
    StateMachine();
    ~StateMachine();

    // 상태를 등록
    void addState(std::unique_ptr<State> state);

    // 초기 상태를 설정
    void setInitial(const std::string& name);

    // 다른 상태로 전이
    void transitionTo(const std::string& name);

    // 현재 상태의 onUpdate() 를 호출
    void update();

    // 현재 상태의 이름을 리턴
    std::string currentName() const;

    using Guard = std::function<bool()>;
    using TransitionCallback = std::function<void(const std::string& from,
                                                  const std::string& to,
                                                  const std::string& event)>;
        
    struct Transition {
        std::string from;
        std::string to;
        std::string event;
        Guard guard; 
    };

    void addTransition(const std::string& from,
                       const std::string& to,
                       const std::string& event,
                       Guard guard = nullptr);

    bool handleEvent(const std::string& event);

    void setOnTransition(TransitionCallback cb);        

private:
    std::unordered_map<std::string, std::unique_ptr<State>> states_;
    State* current_ = nullptr;

    static std::string makeKey(const std::string& from, const std::string& event);
    std::unordered_map<std::string, std::vector<Transition>> transitions_;
    TransitionCallback onTransition_;
};

} // namespace fsm
