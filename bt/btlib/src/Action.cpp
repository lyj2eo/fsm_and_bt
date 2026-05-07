#include "bt/Action.h"

namespace bt {

Status Action::tick() {
    // TODO:
    //   func_ 가 비어 있지 않다면 호출하고 그 결과를 돌려준다.
    //   비어 있다면 Failure 를 돌려주는 게 안전하다.
    return Status::Failure;
}

} // namespace bt
