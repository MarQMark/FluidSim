#ifndef FLUIDSIM_SIMULATION_SYSTEM_H
#define FLUIDSIM_SIMULATION_SYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"
#include "FluidSim/Grid.h"
#include "FluidSim/DistanceField.h"
#include "FluidSim/Constants.h"
#include "Kikan/ecs/Scene.h"
#include <map>

class SimulationSystem : public Kikan::ISystem{
public:
    explicit SimulationSystem(DistanceField* distanceField, Constants* constants, Kikan::Scene* scene);
    ~SimulationSystem() override;

    void update(double dt) override;

    void setDistanceField(DistanceField* distanceField);
private:
    Constants* _constants;
    Grid* _grid;
    DistanceField* _distanceField;
    std::map<Particle*, std::vector<Particle*>> _p_neighbours;

    Kikan::Scene* _scene;

    void apply_external_forces(float dt);
    void apply_viscosity(float dt);
    void advance_particles(float dt);
    void update_neighbours();
    void double_density_relaxation(float dt);
    void resolve_collisions(float dt);
    void update_velocity(float dt);
    void update_sprite();
};

#endif //FLUIDSIM_SIMULATION_SYSTEM_H
