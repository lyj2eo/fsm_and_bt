# 03. btlib 확장 — Decorator (Inverter / Retry)

## 목표
"자식 노드 하나"를 감싸서 결과를 가공하는 **Decorator** 노드를 라이브러리에 추가한다.
가장 흔한 두 종류만 만든다.

- **Inverter** — 자식의 `Success` ↔ `Failure` 를 뒤집는다.
- **Retry**   — 자식이 `Failure` 를 돌려주면 최대 N 번까지 다시 tick 한다. 한 번이라도
                `Success` 면 즉시 `Success`. N 번 모두 실패하면 `Failure`.

## 라이브러리에 추가
`bt/btlib/include/bt/` 에 새 헤더를, `bt/btlib/src/` 에 새 소스를 만든다.
GLOB 가 자동으로 잡아주니 CMake 수정은 필요 없다.

추천 시그니처:

```cpp
// Decorator.h — 자식 1개를 가지는 노드의 베이스
#pragma once
#include "Node.h"
#include <memory>

namespace bt {
class Decorator : public Node {
public:
    void setChild(std::unique_ptr<Node> child);
protected:
    std::unique_ptr<Node> child_;
};
} // namespace bt
```

```cpp
// Inverter.h
#pragma once
#include "Decorator.h"

namespace bt {
class Inverter : public Decorator {
public:
    Status tick() override;
    std::string name() const override { return "Inverter"; }
};
} // namespace bt
```

```cpp
// Retry.h
#pragma once
#include "Decorator.h"

namespace bt {
class Retry : public Decorator {
public:
    explicit Retry(int max_attempts);
    Status tick() override;
    std::string name() const override { return "Retry"; }
private:
    int max_attempts_;
};
} // namespace bt
```

본체(`.cpp`)는 학습자가 작성한다. 단순한 흐름이다.
- `Decorator::setChild` : `std::move` 로 받아서 `child_` 에 보관.
- `Inverter::tick` : `child_->tick()` 결과를 뒤집어 반환.
- `Retry::tick` : `for` 루프 안에서 `child_->tick()` 호출, 결과에 따라 처리.

## 시나리오 (peg-in-hole 삽입 + 재시도)
```
Sequence (root)
├── EnableForceMode
├── ApproachHole
├── Retry(3) : InsertAttempt   -- 정렬 오차 때문에 가끔 실패. 최대 3번 재시도
├── VerifyDepth
└── DisableForceMode
```

`InsertAttempt` 는 매 시도마다 정렬이 살짝 보정되어, 첫 1~2번은 측면 충돌(`force_xy` 큼)
때문에 실패하고 마지막 시도에서 성공하도록 시뮬레이션한다.

## 작업할 파일
- `bt/btlib/include/bt/Decorator.h, Inverter.h, Retry.h` — 새로 만든다.
- `bt/btlib/src/Decorator.cpp, Inverter.cpp, Retry.cpp` — 새로 만든다.
- `main.cpp` — 시나리오를 작성한다.

## 도전 (선택)
- `Inverter` 를 실제 시나리오에 끼워 넣어 보라. 예를 들어
  "ContactDetected" 액션을 `Inverter` 로 감싸 "시작 시 접촉이 없는지" 를 사전 검사하는 식.

## 기대 출력 예시
```
[EnableForceMode] on
[ApproachHole] over hole
[InsertAttempt #1] Fxy = 8.4  -> too high, FAIL
[InsertAttempt #2] Fxy = 4.1  -> still off, FAIL
[InsertAttempt #3] Fxy = 1.2  -> inserted
[VerifyDepth] depth ok
[DisableForceMode] off
result = Success
```
