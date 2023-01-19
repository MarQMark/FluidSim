#include <iostream>
#include "FluidSim/FluidSimulation.h"

int WinMain() {
    std::string fileName("assets/tube");

    FluidSimulation fluidSimulation(fileName);

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