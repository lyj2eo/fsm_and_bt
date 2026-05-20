#include "bt/Sequence.h"
#include "bt/Action.h"
#include "bt/Inverter.h"
#include "bt/Retry.h"

#include <iostream>
#include <memory>

namespace {

const char* toStr(bt::Status s) {
    switch (s) {
        case bt::Status::Success: return "Success";
        case bt::Status::Failure: return "Failure";
        case bt::Status::Running: return "Running";
    }
    return "?";
}

std::unique_ptr<bt::Action> checkContact(bool is_contact) {
    return std::make_unique<bt::Action>("CheckNoContact", [is_contact] {
        if (is_contact) {
            std::cout << ">> contact is detected!\n";
            return bt::Status::Failure;
        }

        std::cout << ">> contact is not detected!\n";
        return bt::Status::Success;
    });
}

} // namespace

int main() {
    bool is_contact = false;
    int attempt_cnt = 0;       // current attempt count
    int max_attempt_cnt = 5;   // maximum attempt count
    double force_xy = 0.0;
    const double align_force_thresh = 2.0;

    bt::Sequence root("root", bt::Sequence::SequenceType::Resume);

    /* * * * add actions for sequence * * * */
    // 1. 접촉 확인 (접촉이 있을시 실패)
    
    // 2. enable force mode
    auto enableForceMode = std::make_unique<bt::Action>("EnableForceMode", []{
        std::cout << ">> force control on!\n";
        return bt::Status::Success;
    });

    // 3. 접촉 확인 (접촉이 없을시 실패)

    // 4. approach hole
    auto approachHole = std::make_unique<bt::Action>("ApproachHole", []{
        std::cout << ">> approach the hole\n";
        return bt::Status::Success;
    });

    // 5. insert attempt (Retry 3times)
    auto insertAttempt = std::make_unique<bt::Action>("InsertAttempt", [&]{
        ++attempt_cnt;
        
        force_xy = 10 - 4*(attempt_cnt - 1);
        std::cout << ">> [InsertAttempt #" << attempt_cnt << "] Fxy = " << force_xy;
        if (force_xy > align_force_thresh) {
            std::cout << "  -> too high, FAIL\n";
            return bt::Status::Failure;
        }
        std::cout << "  -> inserted\n";
        return bt::Status::Success;
    });

    // 6. verify depth
    auto verifyDepth = std::make_unique<bt::Action>("VerifyDepth", []{
        std::cout << ">> insertion depth ok\n";
        return bt::Status::Success;
    });

    // 7. disable force mode
    auto disableForceMode = std::make_unique<bt::Action>("DisableForceMode", []{
        std::cout << ">> force control off!\n";
        return bt::Status::Success;
    });



    /* * * * add children to sequence * * * */
    root.addChild(checkContact(is_contact));

    root.addChild(std::move(enableForceMode));

    // for inverter test
    is_contact = true;
    auto inverter = std::make_unique<bt::Inverter>("Inverter");
    inverter->addChild(checkContact(is_contact));
    root.addChild(std::move(inverter));

    root.addChild(std::move(approachHole));

    // for retry test
    auto retry = std::make_unique<bt::Retry>("Retry", max_attempt_cnt);
    retry->addChild(std::move(insertAttempt));
    root.addChild(std::move(retry));

    root.addChild(std::move(verifyDepth));
    root.addChild(std::move(disableForceMode));



    /* * * * run scenario * * * */
    bt::Status status;
    do {
        status = root.tick();
    } while (status == bt::Status::Running);

    std::cout << "result = " << toStr(status) << "\n";
    return 0;
}
