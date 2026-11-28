#ifndef LRCOMPONENTREGISTRY_HPP
#define LRCOMPONENTREGISTRY_HPP

#include "Components/Component.hpp"
#include "Entity.hpp"
#include "../Core/Core.hpp"
#include "ECSArchitectureData.hpp"

#include "Components/Transform.hpp"

#include <memory>
#include <type_traits>


namespace LR {
    /*namespace Components {
        class Component;
    }*/

    class ComponentRegistry {
    public:
        uint64_t GetComponentID(const std::string& name) {
            std::string normalizedName = NormalizeName(name);
            auto it = m_CachedComponentIDs.find(normalizedName);
            if (it != m_CachedComponentIDs.end()) {
                return it->second;
            }

            uint64_t id = m_NextID;
            m_NextID <<= 1;
            m_CachedComponentIDs[normalizedName] = id;
            Core::Debug::Log(std::format("Caching Component ->\tName: {}\t\tID: {}", ComponentRegistry::NormalizeName(normalizedName), id));

            return id;
        }

        uint64_t GetComponentID(const std::vector<std::string>& names) {
            uint64_t id = 0;

            for (const std::string& name : names) id += GetComponentID(name);

            return id;
        }

        static std::string NormalizeName(const std::string& str) {
            size_t pos = str.find_last_of(':');

            if (pos == std::string::npos)
                return str;

            return str.substr(pos + 1);
        }

        template <typename T> requires std::derived_from<T, Components::Component>
        std::shared_ptr<T> AddComponent(Entity& entity) {
            std::shared_ptr<T> obj = std::make_shared<T>();

            uint64_t arcid = entity.GetArchitectureID();
            uint64_t index = 0;

            auto it = m_CachedComponents.find(arcid);
            if (it != m_CachedComponents.end()) {
                ECSArchitectureData data = m_CachedComponents[arcid];

                if (!data.FreeIDs.empty()) {
                    index = *data.FreeIDs.begin();
                    data.FreeIDs.erase(data.FreeIDs.begin());
                }
                else {
                    index = data.Storage.size();
                }

                data.Storage.push_back(obj);

                m_CachedComponents[arcid] = data;
            }
            else {
                ECSArchitectureData data;
                data.Storage.push_back(obj);
                m_CachedComponents[arcid] = data;
            }

            entity.SetIndex(index);
            return obj;
        }

        template <typename T> requires std::derived_from<T, Components::Component>
        std::shared_ptr<T> GetComponent(Entity& entity) {
            uint64_t arcid = entity.GetArchitectureID();
            uint64_t index = entity.GetIndex();

            // Just for testing, TODO: Error handling
            ECSArchitectureData data = m_CachedComponents[arcid];
            return std::dynamic_pointer_cast<T>(data.Storage[index]);
        }

        uint64_t CalculateArchitectureID(const Entity& entity) {
            return GetComponentID(entity.GetComponents());
        }

        template<typename T> requires std::derived_from<T, Components::Component>
        uint64_t GetComponentID() {
            std::string name = typeid(T).name();
            return GetComponentID(name);
        }

    private:
        uint64_t m_NextID = 1;
        std::unordered_map<std::string, uint64_t> m_CachedComponentIDs;

        std::unordered_map<uint64_t, ECSArchitectureData> m_CachedComponents;
    };

}


#endif