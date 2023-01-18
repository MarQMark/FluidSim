#ifndef FLUIDSIM_CONSTANTS_H
#define FLUIDSIM_CONSTANTS_H

struct Constants{
    float RADIUS = 5;
    float SIGMA = .02f;
    float BETA = .05f;
    float P0 = 1000;
    float STIFFNESS = .00000005f;
    float STIFFNESS_NEAR = .00000003f;
    double MAX_PRESS = 1e5;
    float MAX_D = 10;
    float FRICTION = .008f;
    int COLLISION_RADIUS = 2;
};

#endif //FLUIDSIM_CONSTANTS_H
