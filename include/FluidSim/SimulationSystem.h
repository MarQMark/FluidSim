#ifndef FLUIDSIM_SIMULATION_SYSTEM_H
#define FLUIDSIM_SIMULATION_SYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"
#include "FluidSim/Grid.h"
#include "FluidSim/DistanceField.h"
#include <map>

class SimulationSystem : public Kikan::ISystem{
public:
    explicit SimulationSystem(DistanceField* distanceField);
    ~SimulationSystem() override;

    void update(double dt) override;

private:
    Grid* _grid;
    DistanceField* _distanceField;
    std::map<Particle*, std::vector<Particle*>> _p_neighbours;

    void apply_external_forces(double dt);
    void apply_viscosity(double dt);
    void advance_particles(double dt);
    void update_neighbours();
    void double_density_relaxation(double dt);
    void resolve_collisions();
    void update_velocity(double dt);
    void update_sprite();
};

#endif //FLUIDSIM_SIMULATION_SYSTEM_H
