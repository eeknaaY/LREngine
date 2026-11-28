#ifndef LRECSARCHITECTUREDATA_HPP
#define LRECSARCHITECTUREDATA_HPP

#include "Components/Component.hpp"

#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace LR {
    class ECSArchitectureData {
    public:
        std::unordered_set<uint64_t> FreeIDs;
        //TODO: Fix
        std::vector<std::shared_ptr<Components::Component>> Storage;
    };
}


#endif