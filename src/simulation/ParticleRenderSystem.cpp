#include "FluidSim/ParticleRenderSystem.h"
#include "FluidSim/Particle.h"
#include "Kikan/util/Timer.h"


ParticleRenderSystem::ParticleRenderSystem(Controls *controls, Constants* constants, Stats *stats) :
    _controls(controls), _constants(constants), _stats(stats) {
    singleInclude(Particle);
}


void ParticleRenderSystem::update(double dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Render Particles"], Kikan::Timer::Precision::MICRO);

    if(_controls->RENDER_MODE != Controls::RMT::PARTICLES)
        return;

    update_vertices();
}

void ParticleRenderSystem::update_vertices() {
    // Gen new Vertices if new Particles were generated
    if(_entities.size() * 4 > _vertices.size()){
        int oldSize = (int)_vertices.size();
        _vertices.resize(_entities.size() * 4);
        _indices.resize(_entities.size() * 6);

        for(int i = oldSize / 4; i < _vertices.size() / 4; i++){

            _vertices[4 * i + 0].textureCoords = glm::vec2(0, 1);
            _vertices[4 * i + 1].textureCoords = glm::vec2(1, 1);
            _vertices[4 * i + 2].textureCoords = glm::vec2(1, 0);
            _vertices[4 * i + 3].textureCoords = glm::vec2(0, 0);

            for (int j = 0; j < 4; ++j) {
                _vertices[4 * i + j].color = glm::vec4(0, 0, .8, 1.);
                _vertices[4 * i + j].texture = 0;
            }

            _indices[6 * i + 0] = 0 + 4 * i;
            _indices[6 * i + 1] = 1 + 4 * i;
            _indices[6 * i + 2] = 2 + 4 * i;
            _indices[6 * i + 3] = 0 + 4 * i;
            _indices[6 * i + 4] = 2 + 4 * i;
            _indices[6 * i + 5] = 3 + 4 * i;
        }
    }
    // Shirk down to number of particles if Particles were destroyed
    else if(_entities.size() * 4 < _vertices.size()){
        _vertices.resize(_entities.size() * 4);
        _vertices.shrink_to_fit();
        _indices.resize(_entities.size() * 6);
        _indices.shrink_to_fit();
    }

    //update Position of Vertices
    for (int i = 0; i < _entities.size(); ++i) {
        auto* p = _entities[i]->getComponent<Particle>();

        glm::vec3 pos = glm::vec3(p->pos.x, p->pos.y, -.5);

        _vertices[4 * i + 0].position = pos + glm::vec3(-_constants->RENDER_RADIUS, _constants->RENDER_RADIUS, 0);
        _vertices[4 * i + 1].position = pos + glm::vec3(_constants->RENDER_RADIUS, _constants->RENDER_RADIUS, 0);
        _vertices[4 * i + 2].position = pos + glm::vec3(_constants->RENDER_RADIUS, -_constants->RENDER_RADIUS, 0);
        _vertices[4 * i + 3].position = pos + glm::vec3(-_constants->RENDER_RADIUS, -_constants->RENDER_RADIUS, 0);

        int vx = (int)(p->vel.x * 90.f);
        int vy = (int)(p->vel.y * 90.f);

        glm::vec2 col = glm::vec2(vx, vy) / 30.f + .5f;

        _vertices[4 * i + 0].color = glm::vec4(col.x, col.y, 1., 1.);
        _vertices[4 * i + 1].color = glm::vec4(col.x, col.y, 1., 1.);
        _vertices[4 * i + 2].color = glm::vec4(col.x, col.y, 1., 1.);
        _vertices[4 * i + 3].color = glm::vec4(col.x, col.y, 1., 1.);
    }

    _renderer->getBatch(0)->overrideVertices(_vertices, _indices);
}
