#include "Kikan/ecs/TypeRegistry.h"

namespace Kikan {
    int TypeRegistry::_last_signature = 0;
    std::map<std::string, unsigned int> TypeRegistry::_component_signatures;
}