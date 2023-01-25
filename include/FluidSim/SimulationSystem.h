#ifndef FLUIDSIM_SIMULATION_SYSTEM_H
#define FLUIDSIM_SIMULATION_SYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"
#include "FluidSim/Grid.h"
#include "FluidSim/DistanceField.h"
#include "FluidSim/Constants.h"
#include "Kikan/ecs/Scene.h"
#include "Stats.h"
#include "Controls.h"
#include "RenderSystem.h"
#include <map>

class SimulationSystem : public Kikan::ISystem{
public:
    explicit SimulationSystem(DistanceField* distanceField, Constants* constants, Controls* controls, Stats* stats, Kikan::Scene* scene, RenderSystem* rs);
    ~SimulationSystem() override;

    void update(double dt) override;

    void setDistanceField(DistanceField* distanceField);

    Grid* getGrid();
private:
    Grid* _grid;
    DistanceField* _distanceField;
    std::map<Particle*, std::vector<Particle*>> _p_neighbours;

    Constants* _constants;
    Controls* _controls;
    Stats* _stats;
    unsigned int _lost_ps = 0;

    bool _extern_force = false;

    Kikan::Scene* _scene;
    RenderSystem* _rs;

    void apply_controls(float dt);
    void apply_external_forces(float dt);
    void apply_viscosity(float dt);
    void advance_particles(float dt);
    void update_neighbours();
    void double_density_relaxation(float dt);
    void resolve_collisions(float dt);
    void update_velocity(float dt);
    void update_sprite();
    void update_stats();
};

#endif //FLUIDSIM_SIMULATION_SYSTEM_H
