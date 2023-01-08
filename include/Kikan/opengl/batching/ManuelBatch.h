#ifndef KIKAN_MANUEL_BATCH_H
#define KIKAN_MANUEL_BATCH_H

#include "Batch.h"

namespace Kikan {
    class ManuelBatch : Batch{
    public:
        void addVertices();
        void updateVertices();
        void removeVertices();

    };
}

#endif //KIKAN_MANUEL_BATCH_H
