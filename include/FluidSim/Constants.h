#ifndef FLUIDSIM_CONSTANTS_H
#define FLUIDSIM_CONSTANTS_H

struct Constants{
    float RADIUS = 5;
    float SIGMA = .02f;
    float BETA = .05f;
    float P0 = 500;
    float STIFFNESS = 1e-06;
    float STIFFNESS_NEAR = 3e-07;
    double MAX_PRESS = 1e5;
    float MAX_D = 10;
    float FRICTION = .008f;
    int COLLISION_RADIUS = 2;
    float MAX_VEL = 10.f;

    bool RESET = false;
    bool PAUSE = false;
};

#endif //FLUIDSIM_CONSTANTS_H
