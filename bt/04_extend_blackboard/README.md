# 04. btlib 확장 — Blackboard (노드 간 데이터 공유)

## 목표
지금까지의 액션은 람다가 캡처한 main 의 변수를 직접 읽고 썼다. 트리가 커지면 이런
방식은 곧 한계에 부딪히므로, BT 에서는 보통 **Blackboard** 라는 키-값 저장소를 두고
노드들이 이 저장소를 통해 값을 주고 받는다.

이 단계에서는 가장 단순한 형태의 Blackboard 를 라이브러리에 추가하고, 힘제어 작업의
**측정 → 판단 → 보정** 흐름에서 노드들이 데이터를 주고 받게 만든다.

## 라이브러리에 추가
`bt/btlib/include/bt/Blackboard.h` 에 다음을 만든다.

```cpp
#pragma once
#include <string>
#include <unordered_map>

namespace bt {

class Blackboard {
public:
    void   set(const std::string& key, double value);
    double get(const std::string& key, double default_value = 0.0) const;
    bool   has(const std::string& key) const;

private:
    std::unordered_map<std::string, double> doubles_;
};

} // namespace bt
```

본체(`Blackboard.cpp`)에서 `unordered_map` 만 적당히 다루면 된다.

> **메모.** 실제 BT 라이브러리들은 임의 타입(`std::any`, `std::variant`)을 저장하지만,
> 여기서는 학습용으로 `double` 한 종류만 저장한다. 충분히 흐름을 익힐 수 있다.

## 시나리오 (표면 추종 보정)
협동로봇이 굴곡진 표면을 일정한 힘으로 누르며 이동하려 한다. 매 사이클에서:

```
Sequence (root)
├── EnableForceMode
├── MeasureFz          -- 센서를 읽어 BB["force_z"] 에 저장
├── DecideAdjustment   -- BB["force_z"] 와 목표값을 비교해 BB["dz_correction"] 계산
├── ApplyAdjustment    -- BB["dz_correction"] 만큼 TCP 의 z 를 보정
└── DisableForceMode
```

`MeasureFz` 는 force 를 BB에 **쓰고**, `DecideAdjustment` 는 그것을 **읽어** 보정량을
다시 BB에 **쓴다**. `ApplyAdjustment` 는 보정량을 **읽어** 사용한다.

## 작업할 파일
- `bt/btlib/include/bt/Blackboard.h, src/Blackboard.cpp` — 새로 만든다.
- `main.cpp` — 시나리오를 구성한다.

## 액션과 Blackboard 의 연결
가장 간단한 방법은 main 에서 `std::shared_ptr<bt::Blackboard>` 를 하나 만들고
모든 액션 람다가 이걸 캡처해서 사용하는 것이다.

```cpp
auto bb = std::make_shared<bt::Blackboard>();
auto act = std::make_unique<bt::Action>("MeasureFz", [bb]{
    double fz = /* 센서 읽기 */;
    bb->set("force_z", fz);
    return bt::Status::Success;
});
```

## 기대 출력 예시
```
[EnableForceMode] on
[MeasureFz] Fz = 7.2
[DecideAdjustment] target=5.0  err=2.2  dz_correction=-0.011
[ApplyAdjustment] z -= 0.011
[DisableForceMode] off
result = Success
```
