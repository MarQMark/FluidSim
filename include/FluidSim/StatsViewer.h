#ifndef FLUIDSIM_STATS_VIEWER_H
#define FLUIDSIM_STATS_VIEWER_H

#include "Stats.h"

class StatsViewer {
public:
    StatsViewer();
    ~StatsViewer();

    void render();

    Stats* getStats();
private:
    Stats* _stats;
};

#endif //FLUIDSIM_STATS_VIEWER_H
