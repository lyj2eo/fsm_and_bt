# 04. 협동로봇 안전상태 FSM

## 목표
03 단계에서 추가한 **전이 맵(transition map)** + **이벤트 처리** + **가드** + **콜백**을
모두 활용해 협동로봇의 안전 상태 머신을 구현한다. 라이브러리에는 손대지 않는다.

## 도메인
협동로봇은 사람과 같은 작업 공간을 공유하기 때문에, 외부 이벤트에 따라 운영 모드를
바꿔야 한다. 단순화한 모델을 사용한다.

| 상태 | 의미 |
|---|---|
| `Normal` | 정상 운전. 정격 속도. |
| `ReducedSpeed` | 사람이 안전 영역 안에 있음. 속도를 일정 비율로 낮춤. |
| `ProtectiveStop` | 안전 정지. 모터는 살아 있으나 동작 정지. 해제 입력으로 복귀. |
| `EmergencyStop` | 비상 정지. 물리 e-stop 버튼. 리셋 후 재시동 절차 필요. |

## 전이 맵
정상 운영 두 상태는 `human_in_zone` / `zone_clear` 로 오간다.

```
   ┌────────┐  human_in_zone   ┌──────────────┐
   │ Normal │ ---------------→ │ ReducedSpeed │
   │        │ ←--------------- │              │
   └────────┘    zone_clear    └──────────────┘
```

위의 두 운영 상태에서 다음 이벤트가 추가로 발화 가능하다.

| from                                         | event                | to               | guard            |
|----------------------------------------------|----------------------|------------------|------------------|
| `Normal`, `ReducedSpeed`                     | `collision_detected` | `ProtectiveStop` | (없음)           |
| `ProtectiveStop`                             | `reset`              | `Normal`         | (없음)           |
| `Normal`, `ReducedSpeed`, `ProtectiveStop`   | `estop_pressed`      | `EmergencyStop`  | (없음)           |
| `EmergencyStop`                              | `reset`              | `Normal`         | `!estop_pressed` |

핵심 포인트:
- `collision_detected` 와 `estop_pressed` 는 여러 from 에서 받을 수 있다 — 각 from
  마다 `addTransition` 을 한 번씩 부르면 된다.
- `EmergencyStop → Normal` 만 **가드** 가 있다. 물리 e-stop 버튼이 release 된
  (`!estop_pressed`) 상태에서만 `reset` 으로 빠져나갈 수 있다.

## 작업할 파일
- `main.cpp` — 4개 상태 + 전이 맵 등록 + 시나리오 작성.
- 라이브러리는 수정하지 않는다.

## 시나리오
main 에서 다음 이벤트를 순서대로 던진다.

1. 시작: `Normal`
2. `human_in_zone` → `ReducedSpeed`
3. `zone_clear` → `Normal`
4. `collision_detected` → `ProtectiveStop`
5. `reset` → `Normal`
6. `estop_pressed` → `EmergencyStop` (이때 `estop_pressed` 변수 = true)
7. `reset` 시도 — 가드에 막혀 거부됨
8. `estop_pressed` 변수 = false 후 `reset` → `Normal`

전이 콜백으로 매 발화를 한 줄씩 출력. `handleEvent` 가 false 를 돌려준 경우(가드
거부) 도 명시적으로 출력한다.

## 힌트
- 같은 이벤트를 여러 from 에서 받게 하려면 `addTransition` 을 from 마다 한 번씩 부른다.
  ```cpp
  for (const char* from : {"Normal", "ReducedSpeed"}) {
      sm.addTransition(from, "ProtectiveStop", "collision_detected");
  }
  ```
- 가드는 `[&]{ return !estop_pressed; }` 처럼 main 변수를 캡처해 표현한다.
- 입력은 `bool estop_pressed` 같은 변수를 main 스코프에 두고 단계마다 값만 바꾸면 된다.

## 기대 출력 예시
```
  enter  Normal
[T] Normal -> ReducedSpeed (human_in_zone)
[T] ReducedSpeed -> Normal (zone_clear)
[T] Normal -> ProtectiveStop (collision_detected)
[T] ProtectiveStop -> Normal (reset)
[T] Normal -> EmergencyStop (estop_pressed)
event 'reset' rejected: e-stop still latched
[T] EmergencyStop -> Normal (reset)
final = Normal
```
