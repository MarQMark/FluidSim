#ifndef KIKAN_TIME_H
#define KIKAN_TIME_H

namespace Kikan{
    struct Time{
        // Delta time since last frame
        double dt = 0;
        // Total time since Engine creation
        double tt = 0;
        // Interpolated frames per second
        unsigned int fps = 0;
    };
}
#endif //KIKAN_TIME_H
