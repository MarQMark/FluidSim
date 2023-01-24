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
    float PEN_SIZE = 30;
    bool SINGLE_SPAWN = true;
    bool SINGLE_SPAWN_SPAWNED = false;
    float ERASER_SIZE = 50;
    int FORCE_MODE = FMT::M_PULL;
    float FORCE = 0.06;

    bool MOUSE_IN_SPACE = false;
    float MOUSE_X = 0;
    float MOUSE_Y = 0;
};

#endif //FLUIDSIM_CONTROLS_H
