#include <iostream>
#include "FluidSim/FluidSimulation.h"

int WinMain() {
    FluidSimulation fluidSimulation;

    while (fluidSimulation.shouldRun()){
        fluidSimulation.update();
    }

    return 0;
}

#ifdef __linux__
int main(){
    WinMain();
}
#endif