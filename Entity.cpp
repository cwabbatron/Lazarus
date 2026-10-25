#include "Entity.h"

#include "LuaManager.h"

void Entity::update(LuaManager& luamanager)
{
    for (auto& comp : m_components) {
        comp->update(*this);
    }

    for (auto& luacomp : m_lua_components) {
        luamanager.execute_lua_update_on_entity(*this, luacomp);
    }
}
