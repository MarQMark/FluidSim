#ifndef KIKAN_ISYSTEM_H
#define KIKAN_ISYSTEM_H

#include <algorithm>
#include "vector"
#include "Kikan/ecs/TypeRegistry.h"
#include "Kikan/ecs/Entity.h"
#include "Kikan/input/Input.h"

namespace Kikan {

#define sig(x) signature<x>()
#define singleInclude(x) includeSignatures(new std::vector<unsigned int>({ sig(x) }))
#define includeAnd(args...) includeSignatures(new std::vector<unsigned int>({ args }))

    class ISystem{
    public:
        ISystem()= default;

        ~ISystem(){
            for (auto signatures : _signatures) {
                delete signatures;
            }
        }

        void setInput(Input* input){
            _input = input;
        }

        virtual void update(double dt) = 0;

        void virtual addEntity(Entity* entity){
            _entities.push_back(entity);
        }

        void virtual removeEntity(Entity* entity){
            for (int i = 0; i < _entities.size(); ++i) {
                if(_entities.at(i) == entity){
                    _entities.erase(_entities.begin() + i);
                    return;
                }
            }
        }

        std::vector<std::vector<unsigned int>*>& getSignatures(){
            return _signatures;
        }

    protected:
        Input* _input;

        template<class T>
        unsigned int signature(){
            return TypeRegistry::getSignature<T>();
        }

        void includeSignatures(std::vector<unsigned int>* signatures){
            std::sort(signatures->begin(), signatures->end());
            _signatures.push_back(signatures);
        }

        std::vector<Entity*> _entities;

    private:
        std::vector<std::vector<unsigned int>*> _signatures;
    };
}
#endif //KIKAN_ISYSTEM_H
