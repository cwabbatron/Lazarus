#include "Entity.h"
#include <iostream>
#include "LuaManager.h"

void print_registry(std::unordered_map<std::string, std::function<std::unique_ptr<Component>()>>& reg) {
    std::cout << "printing components from the registry \n";
    std::for_each(
        reg.begin(),
        reg.end(),
        [](const std::pair<std::string, std::function<std::unique_ptr<Component>()>>& pair) {
            std::cout << "NAME:::::::" <<pair.first << "\n";
        }
    );
}
void Entity::add_component_registry(std::unordered_map<std::string, std::function<std::unique_ptr<Component>()>> &registry, const std::string &name)
{
    std::cout << "Name passed: " << name << "\n";
    
    print_registry(registry);
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
