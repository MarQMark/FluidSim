#include "Kikan/ecs/Scene.h"
#include "Kikan/ecs/systems/IRenderSystem.h"

namespace Kikan {
    std::string Scene::name() {
        return _name;
    }

    bool isSubset(std::vector<unsigned int> &systemSignature, std::vector<unsigned int> &entitySignature) {
        return std::includes(entitySignature.begin(), entitySignature.end(), systemSignature.begin(),
                             systemSignature.end());
    }

    void Scene::addEntity(Entity *entity) {
        _entities.push_back(entity);

        std::vector<unsigned int> entitySignature = entity->getSignatures();
        std::sort(entitySignature.begin(), entitySignature.end());

        for (ISystem *system: _systems) {
            bool containsAll = false;

            for (auto systemSignature: system->getSignatures()) {
                if (isSubset(*systemSignature, entitySignature)) {
                    containsAll = true;
                    break;
                }
            }

            if (containsAll)
                system->addEntity(entity);
        }
    }

    void Scene::removeEntity(Entity *entity) {
        std::vector<unsigned int> entitySignature = entity->getSignatures();
        std::sort(entitySignature.begin(), entitySignature.end());

        for (ISystem *system: _systems) {
            bool containsAll = false;

            for (auto systemSignature: system->getSignatures()) {
                if (isSubset(*systemSignature, entitySignature)) {
                    containsAll = true;
                    break;
                }
            }

            if (containsAll)
                system->removeEntity(entity);
        }

        for (int i = 0; i < (int)_entities.size(); ++i) {
            if (_entities.at(i) == entity) {
                _entities.erase(_entities.begin() + i);
                return;
            }
        }
    }

    void Scene::deleteEntity(Entity *entity) {
        removeEntity(entity);
        delete entity;
    }

    void Scene::addSystem(ISystem *system) {
        if (auto *rSystem = dynamic_cast<IRenderSystem *>(system))
            rSystem->setRenderer(_renderer);

        system->setInput(_input);
        _systems.push_back(system);
    }

    void Scene::update(double dt) {
        _renderer->mvp = _camera->matrix();

        for (ISystem *system: _systems)
            system->update(dt);
    }

    Camera *Scene::camera() {
        return _camera;
    }
}