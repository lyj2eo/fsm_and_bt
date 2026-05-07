#include "bt/Selector.h"

namespace bt {

Status Selector::tick() {
    // TODO:
    //   children_ 를 앞에서부터 tick 한다.
    //   하나라도 Success 면 즉시 Success 반환.
    //   모두 Failure 면 Failure 반환.
    return Status::Failure;
}

} // namespace bt
