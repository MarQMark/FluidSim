#ifndef FLUIDSIM_SIMULATION_SYSTEM_H
#define FLUIDSIM_SIMULATION_SYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"
#include "FluidSim/Grid.h"
#include "FluidSim/DistanceField.h"
#include "FluidSim/Constants.h"
#include "Kikan/ecs/Scene.h"
#include "Stats.h"
#include "Controls.h"
#include "GridRenderSystem.h"
#include <map>
#include <thread>
#include <mutex>

class SimulationSystem : public Kikan::ISystem{
public:
    explicit SimulationSystem(DistanceField* distanceField, Constants* constants, Controls* controls, Stats* stats, Kikan::Scene* scene, GridRenderSystem* rs);
    ~SimulationSystem() override;

    void update(double dt) override;

    void setDistanceField(DistanceField* distanceField);

    Grid* getGrid();
private:
    std::vector<std::thread> _threads;
    std::vector<bool> _sync_id;
    unsigned int _thread_count;
    bool _running = true;
    std::mutex _mut;
    std::mutex _grid_mut;
    std::mutex _neighbor_mut;
    int _sync = -1;

    Grid* _grid;
    DistanceField* _distanceField;
    std::map<Particle*, std::vector<Particle*>> _p_neighbours;

    Constants* _constants;
    Controls* _controls;
    Stats* _stats;
    unsigned int _lost_ps = 0;

    bool _extern_force = false;

    Kikan::Scene* _scene;
    GridRenderSystem* _rs;

    void run(int id);

    void apply_controls(float dt);

    void apply_external_forces(float dt, int id);
    void apply_viscosity(float dt, int id);
    void advance_particles(float dt, int id);
    void update_neighbours(int id);
    void double_density_relaxation(float dt, int id);
    void resolve_collisions(float dt, int id);
    void update_velocity(float dt, int id);

    void update_stats();

    void limit_pos(Particle* p);
};

#endif //FLUIDSIM_SIMULATION_SYSTEM_H
