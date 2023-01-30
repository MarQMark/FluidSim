#ifndef FLUIDSIM_CONSTANTS_H
#define FLUIDSIM_CONSTANTS_H

struct Constants{
    float RADIUS = 3;
    float SIGMA = .02f;
    float BETA = .05f;
    float P0 = 200;
    float STIFFNESS = 1e-06;
    float STIFFNESS_NEAR = 3e-07;
    float FRICTION = .002f;
    int COLLISION_RADIUS = 0;
    float RENDER_RADIUS = 5;
    int RENDER_SMOOTHING = 15;

    unsigned int TEXTURE_ID;
};

#endif //FLUIDSIM_CONSTANTS_H
