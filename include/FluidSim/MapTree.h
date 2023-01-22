#ifndef FLUIDSIM_MAP_TREE_H
#define FLUIDSIM_MAP_TREE_H

#include "MapFile.h"

class MapTree{
public:
    MapTree(std::vector<MapFile*>* maps);

    void render();

    MapFile* getLoaded();
private:

    std::vector<MapFile*>* _maps;
    MapFile* _loaded = nullptr;
};

#endif //FLUIDSIM_MAP_TREE_H
