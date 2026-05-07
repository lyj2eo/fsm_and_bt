# 01. fsmlib 코어 만들기

## 목표
`fsm::StateMachine` 의 비어 있는 멤버 함수들을 채워, 상태들이 제대로 등록되고
전이될 때 라이프사이클(`onEnter` → `onUpdate` → `onExit`)이 올바른 순서로 호출되게 만든다.

## 작업할 파일
- `fsm/fsmlib/src/StateMachine.cpp` — 모든 함수 본체가 비어 있다. 채운다.
- 필요하면 `fsm/fsmlib/include/fsm/StateMachine.h` 의 멤버 변수도 늘려도 된다.

`fsm/01_build_core/main.cpp` 는 **수정하지 않는다.** 이 main 이 기대하는 출력이
나오면 라이브러리가 동작한다는 뜻이다.

## 기대 출력
```
[A] enter
[A] update
[A] exit
[B] enter
[B] update
current = B
```

## 힌트
- `addState` 는 `std::move` 로 unique_ptr 의 소유권을 받아 `states_` 맵에 넣는다.
- `setInitial`, `transitionTo` 는 호출되기 전후로 어떤 상태가 활성인지를 잘 추적해야 한다.
- 등록되지 않은 이름이 들어왔을 때의 동작은 학습자가 정한다(무시 / 메시지 출력 등).
  단, 크래시는 나지 않게 한다.
