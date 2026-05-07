# 05. 통합 시나리오 — 힘제어 기반 peg-in-hole 조립

## 목표
지금까지 만든 모든 도구 — `Sequence`, `Action`, `Retry` (필요시 `Inverter`),
`Blackboard` — 를 한 번에 사용해 **부품을 픽업하고 구멍에 삽입하는** 작업을 BT 로
표현한다.

라이브러리에는 더 이상 새 기능을 추가하지 않는다. **사용** 만 한다.

## 트리 (권장)
```
Sequence (root)
├── EnableForceMode
│
├── Sequence (Pick)
│   ├── MoveAbovePart
│   ├── DescendUntilContact     # bb["surface_z"] = pose_z
│   ├── CloseGripper
│   └── Lift
│
├── MoveAboveHole
│
├── Retry(3) : InsertAttempt    # bb["last_force_xy"] 갱신, 임계치 넘으면 FAIL → 재시도
│
├── VerifyDepth                 # bb["surface_z"] 와 현재 깊이 비교 (sanity check)
├── ReleaseGripper
└── DisableForceMode
```

## 시뮬레이션 권장사항
- 시뮬레이션 변수(`pose_z`, `force_z`, `force_xy`, `gripper_closed` 등)는 main 스코프에 두고,
  액션 람다에서 캡처한다.
- "픽업 시 표면 높이"(접촉이 잡힌 z 값)는 Blackboard 에 저장해, `VerifyDepth` 에서
  꺼내 "삽입 깊이가 표면 높이보다 충분히 깊은가" 같은 검사를 하는 식으로 활용해 본다.
- `InsertAttempt` 는 03번과 비슷하게, 시도를 거듭할수록 정렬이 좋아져 결국 통과하도록 만든다.

## 작업할 파일
- `main.cpp` — 트리 구성과 액션 람다를 모두 작성한다.
- `bt/btlib` 는 수정하지 않는다.

## 평가 포인트 (스스로 점검)
1. 트리의 어느 한 단계라도 `Failure` 면 root 가 `Failure` 로 끝나는가?
2. `Retry` 가 정말 여러 번 시도 후 성공/실패를 가려내는가?
3. `Pick` 단계에서 BB 에 저장한 값을 `VerifyDepth` 가 읽어 사용하는가?
4. "사람 진입 → 안전 정지" 같은 추가 요구사항을 끼워 넣는다면 어디를 어떻게 손대야 할까?
   (이 질문에 대한 답은 다음 단계 — Parallel 노드 등 — 에서 자연스럽게 등장한다. 지금은 생각만.)

## 기대 출력 예시 (자유 양식)
```
[EnableForceMode] on
[MoveAbovePart] z=0.10
[DescendUntilContact] contact at z=0.062 Fz=5.4
  bb[surface_z] = 0.062
[CloseGripper] closed
[Lift] z=0.20
[MoveAboveHole] over hole
[InsertAttempt #1] Fxy=8.2 -> FAIL
[InsertAttempt #2] Fxy=3.5 -> FAIL
[InsertAttempt #3] Fxy=0.8 -> inserted
[VerifyDepth] depth ok
[ReleaseGripper] open
[DisableForceMode] off
result = Success
```
