# 03. fsmlib 확장 — 전이 맵(transition map) 과 콜백

## 목표
지금까지의 `fsmlib` 는 호출자가 `transitionTo(name)` 으로 직접 다음 상태를 지정해야
했다. 이건 어디서 어떤 전이가 가능한지가 코드 흐름에 흩어져 잘 안 보인다.

이번 단계에서는 **(현재상태, 이벤트) → 다음상태** 형태의 전이 맵을 라이브러리에
추가한다. 어떤 전이가 가능한지를 **선언적으로** 한곳에 모으고, 호출자는 이벤트만
던진다 (`handleEvent("collision_detected")`).

## 추가할 기능
**1) 전이 맵 등록** — `addTransition(from, to, event, guard?)`
**2) 이벤트 처리** — `handleEvent(event)` : 현재 상태에서 매칭되는 전이를 찾아 발화.
**3) 전이 콜백** — 전이가 발화할 때마다 한 번 호출되는 사용자 콜백.

### 추천 시그니처
`fsm/fsmlib/include/fsm/StateMachine.h` 에 다음을 추가하기를 권장한다 (이름/구조는 자유).

```cpp
#include <functional>

namespace fsm {
class StateMachine {
public:
    // ... 기존 멤버 ...

    using Guard = std::function<bool()>;
    using TransitionCallback = std::function<void(
        const std::string& from,
        const std::string& to,
        const std::string& event)>;

    // 전이 등록. event 가 from 상태에서 발생하고 guard 가 통과하면 to 로 간다.
    // guard 는 옵션 — nullptr 이면 항상 통과.
    void addTransition(const std::string& from,
                       const std::string& to,
                       const std::string& event,
                       Guard guard = nullptr);

    // 이벤트 처리. 현재 상태에서 매칭되는 (event) 전이를 찾는다.
    //   - 가드까지 통과하는 전이를 찾으면 그걸 발화하고 true 반환.
    //   - 매칭이 아예 없거나 모든 가드가 거부하면 false 반환.
    bool handleEvent(const std::string& event);

    // 발화 직후(새 상태의 onEnter 다음) 한 번 호출.
    void setOnTransition(TransitionCallback cb);
};
} // namespace fsm
```

### 구현 힌트
- 전이는 보통 `std::unordered_multimap<std::string /*from*/, Transition>` 또는
  `unordered_map<from, vector<Transition>>` 으로 보관한다. `Transition` 구조체는
  `to`, `event`, `guard` 세 필드.
- `handleEvent` 는 현재 상태 기준으로 후보를 모두 가져와 event 가 일치하고 guard 도
  통과하는 첫 항목을 발화한다. 같은 (from, event) 가 여러 개 등록된 경우 가드로 분기.
- 발화 절차는 기존 `transitionTo` 와 동일: `current->onExit()` → 상태 교체 →
  `current->onEnter()` → 콜백 호출.
- 기존 `transitionTo` 는 그대로 둬도 되고, "직접 호출" 의 의미를 살리려면 그대로
  두면서 콜백만 호출하도록 손봐도 된다.

## 시나리오 (협동로봇 운영 모드)
```
   ┌──────┐  start  ┌──────────────┐  calibrated?  ┌───────┐ run ┌───────────┐
   │ Idle │ ------→ │ Initializing │ ------------→ │ Ready │ --→ │ Executing │
   └──────┘         └──────────────┘               └───────┘     └───────────┘
                                                       ↑    done        │
                                                       └────────────────┘
```

이벤트는 다음과 같이 잡는다.

| from           | event         | to           | guard        |
|----------------|---------------|--------------|--------------|
| Idle           | `start`       | Initializing | (없음)       |
| Initializing   | `tick`        | Ready        | `calibrated` |
| Ready          | `run`         | Executing    | (없음)       |
| Executing      | `done`        | Ready        | (없음)       |

- `tick` 은 "주기적 자기 평가" 의미. 캘리브레이션 미완료 상태에서 `tick` 을 던지면
  가드에 막혀 발화하지 않는다.
- 모든 발화 시 콜백이 한 줄씩 로그를 출력한다 (예: `[T] Idle -> Initializing (start)`).

## 작업할 파일
- `fsm/fsmlib/include/fsm/StateMachine.h` — 위 시그니처 추가.
- `fsm/fsmlib/src/StateMachine.cpp` — 본체 구현.
- `main.cpp` — 4개 상태 + 시나리오 작성.

## 기대 출력 예시
```
[T] Idle -> Initializing (start)
event 'tick' rejected: not calibrated yet
[T] Initializing -> Ready (tick)
[T] Ready -> Executing (run)
[T] Executing -> Ready (done)
```
