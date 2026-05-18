// 01_build_core - btlib 의 핵심 노드들이 동작하는지 확인.
//
// 수정하지 않는다. btlib/src/*.cpp 를 채우면 아래 시나리오가 의도대로 동작한다.

#include "bt/Sequence.h"
#include "bt/Selector.h"
#include "bt/Action.h"

#include <iostream>
#include <memory>

namespace {

const char* toStr(bt::Status s) {
    switch (s) {
        case bt::Status::Success: return "Success";
        case bt::Status::Failure: return "Failure";
        case bt::Status::Running: return "Running";
    }
    return "Unknown";
}

std::unique_ptr<bt::Action> ok(const char* tag) {
    return std::make_unique<bt::Action>(tag, [tag] {
        std::cout << "[" << tag << "] run\n";
        return bt::Status::Success;
    });
}

std::unique_ptr<bt::Action> fail(const char* tag) {
    return std::make_unique<bt::Action>(tag, [tag] {
        std::cout << "[" << tag << "] FAIL\n";
        return bt::Status::Failure;
    });
}

std::unique_ptr<bt::Action> running(const char* tag) {
    return std::make_unique<bt::Action>(tag, [tag] {
        static int cnt = 0;
        cnt ++;
        std::cout << "[" << tag << "] RUNNING\n";
        if (cnt >= 2){
            return bt::Status::Success;
        }
        return bt::Status::Running;
    });
}
} // namespace

int main() {
    
    std::cout << "== test 1 : sequence all-success ==\n";
    bt::Sequence seq1;
    seq1.addChild(ok("A"));
    seq1.addChild(ok("B"));
    seq1.addChild(ok("C"));
    bt::Status result = seq1.tick();
    std::cout << "result = " << toStr(result) << "\n\n";



    std::cout << "== test 2 : sequence with failure ==\n";
    bt::Sequence seq2;
    seq2.addChild(ok("A"));
    seq2.addChild(fail("X"));
    seq2.addChild(ok("C"));
    bt::Status result2 = seq2.tick();
    std::cout << "result = " << toStr(result2) << "\n\n";



    std::cout << "== test 3 : selector finds first success ==\n";
    bt::Selector sel1;
    sel1.addChild(fail("X"));
    sel1.addChild(ok("B"));
    sel1.addChild(ok("C"));
    bt::Status result3 = sel1.tick();
    std::cout << "result = " << toStr(result3) << "\n\n";



    std::cout << "== test 4 : selector all fail ==\n";
    bt::Selector sel2;
    sel2.addChild(fail("X"));
    sel2.addChild(fail("X"));
    bt::Status result4 = sel2.tick();
    std::cout << "result = " << toStr(result4) << "\n\n";



    std::cout << "== test 5 : selector running ==\n";
    bt::Selector sel3;
    bt::Status result5;
    
    sel3.addChild(fail("X"));
    sel3.addChild(running("X"));
    sel3.addChild(fail("X"));
        
    for (int i = 0; i < 2; ++i) {
        result5 = sel3.tick();

        std::cout << "result = " << toStr(result5) << "\n\n";    
    }
    
    return 0;
}
