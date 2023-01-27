#include "FluidSim/SpriteRenderSystem.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "Kikan/util/Timer.h"

SpriteRenderSystem::SpriteRenderSystem(Controls* controls) : _controls(controls) {
    singleInclude(Kikan::Texture2DSprite);
}

void SpriteRenderSystem::update(double dt) {
    if(_input->keyPressed(Kikan::Key::P))
        Kikan::Timer timer("Render Sprite");

    for (Kikan::Entity *e: _entities) {
        auto *texture2DSprite = e->getComponent<Kikan::Texture2DSprite>();
        _renderer->renderTexture2D(
                texture2DSprite->points[0],
                texture2DSprite->points[1],
                texture2DSprite->points[2],
                texture2DSprite->points[3],
                texture2DSprite->textureID,
                texture2DSprite->color,
                texture2DSprite->layer);

        // This is bad but with this only the background gets rendered
        if(_controls->RENDER_MODE != Controls::RMT::PARTICLES)
            return;
    }
}
