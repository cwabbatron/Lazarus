#include "Entity.h"
#include <iostream>
#include "LuaManager.h"


void Entity::add_component_name(std::unordered_map<std::string, std::function<std::unique_ptr<Component>()>> &registry, const std::string &name)
{
    
    
    
    std::unique_ptr<Component> comp = registry.at(name)();

    comp->on_attach(*this);

    add_component_unique_ptr(std::move(comp));
}

void Entity::update(LuaManager &luamanager,EntityManager& entmanager)
{
    for (auto& comp : m_components) {
        comp->update(*this);
    }

    const std::vector<std::string> lua_components = m_lua_components;
    for (const auto& luacomp : lua_components) {
        if (has_lua_component(luacomp)) {
            luamanager.execute_lua_update_on_entity(*this, luacomp,entmanager);
        }
    } 
}

void Entity::remove_component_name(const std::string &name)
{
    m_components.erase(
        std::remove_if(
            m_components.begin(),
            m_components.end(),
            [name](std::unique_ptr<Component>& component) {
                return component->get_name() == name;
            }
        ),
        m_components.end()
    );
}
