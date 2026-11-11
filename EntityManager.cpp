#include "EntityManager.h"

void EntityManager::update(LuaManager &manager)
{
    for (auto& entity : m_entities) {
        entity->update(manager,*this);
    }
}