#ifndef FLUIDSIM_STATS_H
#define FLUIDSIM_STATS_H

#include <map>

struct Stats{
    unsigned int PARTICLES;
    unsigned int LOST_PARTICLES;
    unsigned int FPS;

    std::map<std::string, long long> PERFORMANCE;
};

#endif //FLUIDSIM_STATS_H
