#pragma once

#include "Entity.h"
#include <vector>
#include <algorithm>
#include <iostream>
#define MAX_ENTITIES 1024
class EntityManager {
    public:


        Entity* add_entity() {
            if (m_entities.size() + 1 > 1024) [[unlikely]] {
                std::cout << "ERROR!! too many entities";
                return nullptr;
            }
            return &m_entities.emplace_back(m_entities.size() + 1);
        }

        void remove_entity(unsigned int id) {
            m_entities.erase(
                std::remove_if(
                    m_entities.begin(),
                    m_entities.end(),
                    [id](const Entity& entity){
                        return entity.get_id() == id;
                    }
                )
            );
        }

        const std::vector<Entity>& get_entities() const {
            return m_entities;
        }
        std::vector<Entity>& get_entities() {
            return m_entities;
        }
    private:
        std::vector<Entity> m_entities;
        
};