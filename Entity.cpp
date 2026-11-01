#include "Entity.h"
#include <iostream>
#include "LuaManager.h"


void Entity::add_component_name(std::unordered_map<std::string, std::function<std::unique_ptr<Component>()>> &registry, const std::string &name)
{
    std::cout << "Name passed: " << name << "\n";
    
    
    std::unique_ptr<Component> comp = registry.at(name)();

    comp->on_attach(*this);

    add_component_unique_ptr(std::move(comp));
}

void Entity::update(LuaManager &luamanager)
{
    for (auto& comp : m_components) {
        comp->update(*this);
    }

    for (auto& luacomp : m_lua_components) {
        luamanager.execute_lua_update_on_entity(*this, luacomp);
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
        )
    );
}
