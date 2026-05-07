# FSM & BT 구현 튜토리얼

협동로봇(cobot) 시나리오를 따라가며 **유한 상태 기계(FSM)** 와 **행동 트리(BT)** 의
핵심 라이브러리를 직접 만들고 쓰는 튜토리얼.

## 학습 흐름
각 영역은 다음 두 종류의 step 이 번갈아 나온다.

| 종류 | 하는 일 |
|---|---|
| `XX_build_*` | 라이브러리에 새 코어 기능을 **만든다** |
| `XX_use_*`   | 기존 라이브러리를 **사용해** 협동로봇 시나리오를 표현한다 |
| `XX_extend_*` | 라이브러리에 **새 기능을 추가**한 뒤, 그 기능을 사용해 본다 |

```
fsm/
├── fsmlib/                  # 점진적으로 키워나가는 FSM 라이브러리
├── 01_build_core/           # State / StateMachine 골격 채우기
├── 02_use_gripper/          # 그리퍼 4 상태 FSM
├── 03_extend_hooks/         # 전이 콜백, 가드(guard) 확장
└── 04_use_safety/           # 협동로봇 안전상태 FSM

bt/
├── btlib/                   # 점진적으로 키워나가는 BT 라이브러리
├── 01_build_core/           # Sequence / Selector / Action tick 채우기
├── 02_use_force_touch/      # 힘제어 기반 부드러운 접촉 픽업
├── 03_extend_decorator/     # Decorator (Inverter / Retry) 추가
├── 04_extend_blackboard/    # Blackboard (노드 간 데이터 공유) 추가
└── 05_use_assembly/         # 통합: 힘제어 기반 peg-in-hole 조립
```

각 step 폴더의 `README.md` 가 그 step 의 시나리오/요구사항을 설명한다.

## 개발 환경
- 편집기: **VSCode** (확장: *CMake Tools*, *C/C++*)
- 컴파일러: **MSVC** (Visual Studio Build Tools 또는 VS 가 설치돼 있다면 그 cl.exe)
- 빌드 시스템: CMake + Ninja

VSCode 의 CMake Tools 가 처음 실행될 때 "Kit 선택" 을 묻는다. `Visual Studio …
amd64` 또는 `MSVC … x64` 로 시작하는 항목을 고르면 cl.exe 로 빌드한다.

## 빌드 (VSCode CMake Tools)
1. 폴더(`C:\dev\tutorial\fsm_and_bt`) 를 VSCode 로 연다.
2. `Ctrl+Shift+P` → **CMake: Configure** (Kit 미선택 시 위에서 고른 Kit).
3. 좌측 하단 상태바에서 **Build target** 을 원하는 step (`fsm_01_build_core` 등) 으로
   바꾸고 `F7` 로 빌드, `Shift+F5` 로 실행.

## 빌드 (명령줄)
```cmd
cd C:\dev\tutorial\fsm_and_bt
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target fsm_01_build_core
build\bin\fsm_01_build_core.exe
```

명령줄에서 cl.exe 를 쓰려면 *x64 Native Tools Command Prompt for VS* 에서 실행한다
(또는 일반 cmd 에서 한 번 `vcvars64.bat` 을 호출).

## 진도에 맞춰 step 단위로 빌드
각 step 의 `main.cpp` 는 그 step 에서 **추가 / 구현해야 할** 라이브러리 기능을 사용한다.
즉 라이브러리 확장 전에는 후반부 step 이 컴파일 실패한다 (정상적인 학습 동선).
앞 step 부터 순서대로 풀면서 step 단위로 빌드하는 걸 권장한다.

타깃 이름 = `fsm_01_build_core`, `fsm_02_use_gripper`, `fsm_03_extend_hooks`,
`fsm_04_use_safety`, `bt_01_build_core`, `bt_02_use_force_touch`,
`bt_03_extend_decorator`, `bt_04_extend_blackboard`, `bt_05_use_assembly`.

## 작업 규칙
- `XX_build_*`, `XX_extend_*` step 에서는 **라이브러리(fsmlib / btlib) 를 수정**한다.
  - 이때 main.cpp 는 건드리지 않는다 (의도된 사용 사례 검증용).
- `XX_use_*` step 에서는 **라이브러리는 손대지 않고**, main.cpp 에서 라이브러리를 사용한다.
- 라이브러리에 새 헤더/소스를 추가할 때 CMake 를 다시 실행할 필요는 없다 — `GLOB` 가 자동으로 잡아준다.
  (Visual Studio 가 새 파일을 인식 못하는 경우만 `cmake -S . -B build` 를 한 번 더 돌려준다.)

## 테스트 방법
별도의 테스트 프레임워크는 쓰지 않는다. 각 step 의 README 에 적힌 **기대 출력**과
실행 결과를 비교해 직접 확인한다.

## 라이브러리의 최종 상태
이 튜토리얼이 끝나면 두 라이브러리는 다음과 같은 모양이 된다.

**fsmlib**
- `State` (인터페이스) / `StateMachine` (전이, 라이프사이클 호출)
- 전이 콜백 / 가드(guard) 가 있는 전이

**btlib**
- `Status`, `Node`, `Composite`, `Sequence`, `Selector`, `Action`
- `Decorator`, `Inverter`, `Retry`
- `Blackboard`

여기까지가 "기본" 이다. Running 처리(stateful tick), Parallel, 트리 빌더,
시각화 같은 "실사용" 단계는 후속 과제로 남겨 둔다.
