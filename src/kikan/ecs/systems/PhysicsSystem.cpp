#include "Kikan/ecs/systems/PhysicsSystem.h"
#include "Kikan/ecs/components/Physics.h"

namespace Kikan {
    PhysicsSystem::PhysicsSystem() {
        singleInclude(Physics);
    }

    void PhysicsSystem::update(double dt) {
    }
}

