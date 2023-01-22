#ifndef FLUIDSIM_CONTROLS_H
#define FLUIDSIM_CONTROLS_H

struct Controls{
    enum BRUSH_MODE_TYPE{
        M_SPAWN,
        M_ERASE,
        M_FORCE
    } typedef BMT;

    enum FORCE_MODE_TYPE{
        M_PUSH,
        M_PULL
    } typedef FMT;

    bool RESET = false;
    bool PAUSE = false;
    bool REBUILD = false;
    bool LOADING = false;

    int BRUSH_MODE = BMT::M_SPAWN;
    float PEN_SIZE = 10;
    int PARTICLE_COUNT = 100;
    float ERASER_SIZE = 10;
    int FORCE_MODE = FMT::M_PUSH;
    float FORCE = 0.001;
};

#endif //FLUIDSIM_CONTROLS_H
