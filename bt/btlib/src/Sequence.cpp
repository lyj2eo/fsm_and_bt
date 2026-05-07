#include "bt/Sequence.h"

namespace bt {

Status Sequence::tick() {
    // TODO:
    //   children_ 를 앞에서부터 tick 한다.
    //   하나라도 Failure 면 즉시 Failure 반환.
    //   모두 Success 면 Success 반환.
    //   (이 단계에서는 Running 은 고려하지 않아도 된다.)
    return Status::Failure;
}

} // namespace bt
