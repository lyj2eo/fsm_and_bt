# 02. 힘제어 기반 부드러운 접촉 픽업

## 시나리오
협동로봇이 위에서 부품을 픽업할 때, 부품 표면 높이를 정확히 모를 수 있다.
그래서 위치 제어로 한참 미리 멈추거나 너무 세게 부딪치는 일을 막기 위해
**힘제어 모드를 켜고 천천히 내려가다가 일정한 접촉력이 감지되면 멈춘다**.

이 시나리오를 BT 로 표현해 본다.

```
Sequence (root)
├── EnableForceMode      (Z 축 컴플라이언스 ON)
├── MoveAbovePart        (부품 위 안전 높이로 이동)
├── DescendUntilContact  (천천히 하강하며 Fz 모니터링; 임계치 도달 시 Success)
├── CheckContact         (Fz 가 충분히 큰지 한 번 더 확인)
├── CloseGripper
├── Lift                 (안전 높이로 다시 상승)
└── DisableForceMode
```

## 작업할 파일
- `main.cpp` — 시뮬레이션 변수와 액션들을 정의하고, 위 트리를 구성한다.
- `bt/btlib` 는 수정하지 않는다.

## 알아둘 점
- 이 단계에서도 `Status::Running` 은 쓰지 않는다. `DescendUntilContact` 는
  한 번의 tick 안에서 짧게 루프를 돌면서 시뮬레이션을 진행하고, 접촉이 잡히면
  `Success`, 끝까지 못 잡으면 `Failure` 를 돌려준다.
- 액션 람다 안에서 main 의 시뮬레이션 변수(예: `force_z`, `pose_z`)를 캡처해 사용하면 된다.
- 출력 형식은 자유. 다만 어떤 단계가 실행되었는지, 접촉이 언제 잡혔는지가 보여야 한다.

## 기대 출력 예시 (자유 양식)
```
[EnableForceMode] on
[MoveAbovePart] z = 0.10
[DescendUntilContact]
  z=0.090 Fz=1.5
  z=0.080 Fz=3.0
  z=0.070 Fz=4.5
  z=0.060 Fz=6.0  -> contact!
[CheckContact] Fz = 6.0 >= 5.0 OK
[CloseGripper] closed
[Lift] z = 0.20
[DisableForceMode] off
result = Success
```

## 도전 (선택)
- `MoveAbovePart` 가 실패하는 케이스(예: 좌표가 워크스페이스 밖)에서 root 가
  `Failure` 로 끝나는 걸 확인해 보라. 시퀀스의 단축 평가가 직관적으로 와닿을 것이다.
