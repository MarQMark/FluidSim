#ifndef KIKAN_ICOMPONENT_H
#define KIKAN_ICOMPONENT_H

namespace Kikan {
    struct IComponent {
        virtual void destroy() = 0;
    };
}

#endif //KIKAN_ICOMPONENT_H
