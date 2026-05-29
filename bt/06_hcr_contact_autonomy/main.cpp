#include "bt/Sequence.h"
#include "bt/Blackboard.h"

#include "context/contact_context.h"
#include "observer/observer.h"
#include "identifier/contact_identifier.h"
#include "supervisor/contact_supervisor.h"
#include "actions/contact_detection_actions.h"

#include "virtual_environment.h"
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
    ca::TaskInput task_input;
    bt::Blackboard bb;
    // ca::Observer observer(ctx, bb);
    // ca::Identifier identifier(bb);
    ca::Supervisor supervisor(ctx, task_input,bb);
    ve::VirtualEnv env;

    // initiallize
    // observer.init();
    // identifier.init();
    bb.set("position", 0.0);
    bb.set("force", 0.0);
    bb.set("target_position", 0.0);
    bb.set("target_force", 0.0);
    bb.set("is_contact", false);
    bb.set("task_status", "failure");

    auto root = supervisor.makeScenario();
    bt::Status status;
    do {
        static int cnt = 0;
        cnt++;
        std::cout << "\n=== tick " << cnt << " ===\n";

        // update
        // observer.update();
        // identifier.update();

        status = root.tick();
        
        // update virtual environment
        ve::State target_state;
        target_state.position = bb.get<double>("target_position");
        target_state.force = bb.get<double>("target_force");
        
        env.update(target_state);

        bb.update("position", env.curr_state.position);
        bb.update("force", env.curr_state.force);
        bb.update("is_contact", env.is_contact);

    } while (status == bt::Status::Running);

    std::cout << "result = " << toStr(status) << "\n";
    return status == bt::Status::Success ? 0 : 1;
}
