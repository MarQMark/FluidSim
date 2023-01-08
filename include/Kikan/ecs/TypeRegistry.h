#ifndef KIKAN_TYPE_REGISTRY_H
#define KIKAN_TYPE_REGISTRY_H

#include "map"
#include "string"
#include "typeinfo"

namespace Kikan {
    class TypeRegistry{
    public:

        template<class T>
        static unsigned int getSignature(){
            std::string sig = typeid(T).name();

            if(_component_signatures.count(sig))
                return _component_signatures[sig];

            _component_signatures.insert(std::make_pair(sig, _last_signature++));
            return _last_signature - 1;
        }

    private:
        static std::map<std::string, unsigned int> _component_signatures;
        static int _last_signature;
    };
}

#endif //KIKAN_TYPE_REGISTRY_H
