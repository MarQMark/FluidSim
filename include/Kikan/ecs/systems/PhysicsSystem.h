#ifndef KIKAN_PHYSICS_SYSTEM_H
#define KIKAN_PHYSICS_SYSTEM_H

#include "ISystem.h"

namespace Kikan {
    class PhysicsSystem : public ISystem {
    public:
        PhysicsSystem();

        void update(double dt) override;
    };
}

#endif //KIKAN_PHYSICS_SYSTEM_H
