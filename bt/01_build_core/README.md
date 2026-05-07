# 01. btlib 코어 만들기

## 목표
`btlib` 의 `Sequence`, `Selector`, `Action` 의 `tick()` 본체를 채워, 기본적인 BT
평가가 동작하게 만든다.

## 작업할 파일
- `bt/btlib/src/Action.cpp`
- `bt/btlib/src/Sequence.cpp`
- `bt/btlib/src/Selector.cpp`

`bt/01_build_core/main.cpp` 는 수정하지 않는다. 출력이 아래의 "기대 출력" 과 일치하면 통과.

## 알아둘 점
- 이 단계에서는 `Status::Running` 을 사용하지 않는다. 모든 액션은 즉시
  `Success` 또는 `Failure` 를 반환한다.
- `Sequence` 는 "AND", `Selector` 는 "OR" 로 외워두면 직관적이다.
- `Composite::addChild` 는 이미 구현되어 있다. `children_` 만 잘 순회하면 된다.

## 기대 출력
```
== test 1 : sequence all-success ==
[A] run
[B] run
[C] run
result = Success

== test 2 : sequence with failure ==
[A] run
[X] FAIL
result = Failure

== test 3 : selector finds first success ==
[X] FAIL
[B] run
result = Success

== test 4 : selector all fail ==
[X] FAIL
[X] FAIL
result = Failure
```
