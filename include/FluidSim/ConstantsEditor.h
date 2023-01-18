#ifndef FLUIDSIM_CONSTANTS_EDITOR_H
#define FLUIDSIM_CONSTANTS_EDITOR_H

#include "Constants.h"

class ConstantsEditor{
public:
    ConstantsEditor();
    ~ConstantsEditor();

    void render();

    Constants* getConstants();
private:
    Constants* _constants;

};

#endif //FLUIDSIM_CONSTANTS_EDITOR_H
