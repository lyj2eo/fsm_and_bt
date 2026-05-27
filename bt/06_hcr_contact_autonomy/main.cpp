#include "bt/Sequence.h"
#include "bt/Blackboard.h"

#include "context/contact_context.h"
#include "observer/observer.h"
#include "identifier/contact_identifier.h"
#include "supervisor/contact_supervisor.h"
#include "actions/contact_detection_actions.h"

#include <iostream>
#include <memory>

namespace {

const char* toStr(bt::Status status)
{
    switch (status) {
        case bt::Status::Success: return "Success";
        case bt::Status::Failure: return "Failure";
        case bt::Status::Running: return "Running";
    }
    return "?";
}

} // namespace

int main()
{
    ca::RobotContext ctx;
    bt::Blackboard bb;
    ca::Observer observer(ctx, bb);
    ca::Identifier identifier(bb);
    ca::Supervisor supervisor(ctx, bb);

    // initiallize
    observer.init();
    identifier.init();


    bt::Status status;
    do {
        static int cnt = 0;
        cnt++;
        std::cout << "\n=== tick " << cnt << " ===\n";

        // update
        observer.update();
        identifier.update();
        
        // supervisor
        auto root = supervisor.makeScenario();

        status = root.tick();

    } while (status == bt::Status::Running);

    std::cout << "result = " << toStr(status) << "\n";
    return status == bt::Status::Success ? 0 : 1;
}
