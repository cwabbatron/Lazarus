#pragma once

#include "Entity.h"
#include <vector>
#include <algorithm>
#include <iostream>
#define MAX_ENTITIES 1024
class EntityManager {
    public:


        EntityManager() {
            //TODO: not do this
            m_entities.reserve(MAX_ENTITIES);
        }
        Entity* add_entity() {
            if (m_entities.size() + 1 > MAX_ENTITIES) {
                std::cout << "ERROR!! too many entities";
                return nullptr;
            }
            std::unique_ptr<Entity> ent = std::make_unique<Entity>(m_entities.size() + 1);

            return m_entities.emplace_back(std::move(ent)).get();
        }

        void remove_entity(unsigned int id) {
            const auto new_end = std::remove_if(
                m_entities.begin(),
                m_entities.end(),
                [id](const std::unique_ptr<Entity>& entity) {
                    return entity->get_id() == id;
                }
            );
            m_entities.erase(new_end, m_entities.end());
        }

        const std::vector<std::unique_ptr<Entity>>& get_entities() const {
            return m_entities;
        }
        std::vector<std::unique_ptr<Entity>>& get_entities() {
            return m_entities;
        }

        bool has_entity(int id) {
            std::vector<std::unique_ptr<Entity>>::iterator it = std::find_if(
                m_entities.begin(),
                m_entities.end(),
                [id](const std::unique_ptr<Entity>& entity){
                    return entity->get_id() == id;
                }
            );

            return it != m_entities.end() ? true : false;
        }

        void update(LuaManager& manager);
    private:
        std::vector<std::unique_ptr<Entity>> m_entities;
        
};