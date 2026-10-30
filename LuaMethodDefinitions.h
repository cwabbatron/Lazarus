#pragma once


#include "lua.h"
#include "luacode.h"
#include "lualib.h"

#include "LuaManager.h"

int get_attachment_lua(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    LuaManager* manager = static_cast<LuaManager*>(lua_tolightuserdata(state,lua_upvalueindex(2)));

    manager->convert_entity_to_table(*entity);

    return 1;
}

int get_id_lua(lua_State* state) {

    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    if (!entity) {
        std::cout << "ERROR IN get_id_lua, entity pointer is null.";
        return 0;
    }

    lua_pushinteger(state,entity->get_id());
    return 1;
}

int has_component_lua(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    const char* compname = lua_tostring(state,-1);
    if (!entity) {
        std::cout << "entity is null has component lua";
        return 0;
    }

    lua_pushboolean(state,entity->has_component_name(compname) ? 1 : 0);
    return 1;
}

int add_component_lua(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    LuaManager* manager = static_cast<LuaManager*>(lua_tolightuserdata(state,lua_upvalueindex(2)));

    const char* desired_component = lua_tostring(state,-1);

    if (!entity) {
        std::cout << "entity is null in add_component_lua";
        return 0;
    };

    if (!manager) {
        std::cout << "manager is null add_component_lua";
        return 0;
    }

    //grab the factory function from the registry and call it to get the unique pointer
    entity->add_component_registry(manager->get_registry(),desired_component);

    return 0;
}