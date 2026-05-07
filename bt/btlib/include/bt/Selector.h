#pragma once

#include "Composite.h"

namespace bt {

// 자식들을 순서대로 tick.
//   - 자식이 Success 를 반환하면 즉시 Success.
//   - 모든 자식이 Failure 를 반환하면 Failure.
//   - (Running 의 처리는 학습 후반부에 다룬다.)
class Selector : public Composite {
public:
    Status tick() override;
    std::string name() const override { return "Selector"; }
};

} // namespace bt
