#pragma once

namespace bt {

// 한 번의 tick 결과.
//
//   Success — 이 노드의 작업이 성공으로 끝났다.
//   Failure — 이 노드의 작업이 실패했다.
//   Running — 아직 진행 중. 다음 tick 에서 이어서 평가해야 한다.
//
// 학습 1단계에서는 Running 을 사용하지 않는다 (모든 액션이 즉시 Success/Failure 반환).
// Decorator 단계 이후 Running 을 본격적으로 다룬다.
enum class Status {
    Success,
    Failure,
    Running,
};

} // namespace bt
