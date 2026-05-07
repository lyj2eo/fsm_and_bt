#pragma once

#include <string>

namespace fsm {

// 상태(State) 인터페이스.
//
// 모든 상태는 이 클래스를 상속해 자기 동작을 정의한다.
// 라이프사이클:
//   onEnter()  — 이 상태로 진입할 때 1회 호출
//   onUpdate() — 매 update() 호출마다 1회
//   onExit()   — 이 상태에서 빠져나갈 때 1회 호출
//
// name() 은 상태를 식별하는 고유한 문자열을 반환한다.
class State {
public:
    virtual ~State() = default;

    virtual void onEnter() = 0;
    virtual void onUpdate() = 0;
    virtual void onExit() = 0;

    virtual std::string name() const = 0;
};

} // namespace fsm
