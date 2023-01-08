#ifndef KIKAN_I_RENDER_SYSTEM_H
#define KIKAN_I_RENDER_SYSTEM_H

#include "ISystem.h"
#include "Kikan/opengl/Renderer.h"

namespace Kikan {
    class IRenderSystem : public ISystem {
    public:
        IRenderSystem() = default;

        void setRenderer(Renderer* renderer){
            _renderer = renderer;
        }
    protected:
        Renderer* _renderer{};
    };
}

#endif //KIKAN_I_RENDER_SYSTEM_H
