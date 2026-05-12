#pragma once

#include "State.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace fsm {

// 상태들을 보관하고, 현재 상태의 라이프사이클을 호출하는 단순한 FSM.
//
// 사용 예:
//   StateMachine sm;
//   sm.addState(std::make_unique<MyStateA>());
//   sm.addState(std::make_unique<MyStateB>());
//   sm.setInitial("A");          // A.onEnter()
//   sm.update();                 // A.onUpdate()
//   sm.transitionTo("B");        // A.onExit() -> B.onEnter()
//   sm.update();                 // B.onUpdate()
//
// 멤버 함수의 본체는 src/StateMachine.cpp 에 비어 있다. 이 함수들을 구현해서
// 위 사용 예가 동작하도록 만드는 것이 첫 번째 과제(01_build_core)다.
class StateMachine {
public:
    StateMachine();
    ~StateMachine();

    // 상태를 등록한다. 이름은 state->name() 을 사용한다.
    // 같은 이름이 이미 있으면 덮어쓰기 / 무시 / 에러 중 어떻게 다룰지는 학습자가 결정.
    void addState(std::unique_ptr<State> state);

    // 초기 상태를 설정한다. 새 상태의 onEnter() 가 호출된다.
    // 등록되지 않은 이름이면 아무 일도 하지 않거나 메시지를 출력한다.
    void setInitial(const std::string& name);

    // 다른 상태로 전이한다.
    // 현재 상태의 onExit() -> 새 상태의 onEnter() 순으로 호출.
    void transitionTo(const std::string& name);

    // 현재 상태의 onUpdate() 를 호출한다.
    void update();

    // 현재 상태의 이름을 돌려준다. 상태가 없으면 빈 문자열.
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
