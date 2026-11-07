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

int has_component_lua_cpp(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    const char* compname = lua_tostring(state,-1);
    if (!entity) {
        std::cout << "entity is null has component lua";
        return 0;
    }

    lua_pushboolean(state,entity->has_component_name(compname) ? 1 : 0);
    return 1;
}

int has_component_lua_lua(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    LuaManager* manager = static_cast<LuaManager*>(lua_tolightuserdata(state,lua_upvalueindex(2)));

    const char* compname = lua_tostring(state,-1);

    if (!entity) {
        std::cout << "entity is null in has component lua lua";
        return 0;
    }

    std::string file = manager->get_file_from_name_registry(compname);
    
    lua_pushboolean(state,entity->has_lua_component(file));
    return 1;
}

int add_component_lua_lua(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    LuaManager* manager = static_cast<LuaManager*>(lua_tolightuserdata(state,lua_upvalueindex(2)));

    if (!entity || !manager) {
        std::cout << "entity or manager is null in add component lua lua";
        return 0;
    }

    const char* compname = lua_tostring(state,-1);
    std::string filename = manager->get_file_from_name_registry(compname);
    entity->add_lua_component(filename);
    
    return 0;
}

int remove_component_lua_lua(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    LuaManager* manager = static_cast<LuaManager*>(lua_tolightuserdata(state,lua_upvalueindex(2)));

    if (!entity || !manager) {
        std::cout << "entity or manager is null in remove_component_lua_lua";
        return 0;
    }

    const char* compname = lua_tostring(state,-1);

    std::string filename = manager->get_file_from_name_registry(compname);

    entity->remove_lua_component(filename);
    return 0;
}
int add_component_lua_cpp(lua_State* state) {
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

    
    entity->add_component_name(manager->get_registry(),desired_component);

    return 0;
}

int remove_component_lua_cpp(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    

    if (!entity) {
        std::cout << "entity is null in remove_component_lua";
        return 0;
    };

    const char* desired_component = lua_tostring(state,-1);

    entity->remove_component_name(desired_component);

    return 0;
}

int entmanager_add_entity(lua_State* state) {
    EntityManager* manager = static_cast<EntityManager*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    LuaManager* luamanager = static_cast<LuaManager*>(lua_tolightuserdata(state,lua_upvalueindex(2)));

    if (!manager || !luamanager) {
        std::cout << "LuaManager or entity manager is null in entmanager add entity";
        return 0;
    }

    Entity* ent = manager->add_entity();

    if (!ent) {
        lua_pushnil(state);
        return 1;
    }

    luamanager->convert_entity_to_table(*ent);
    return 1;
}

int entmanager_has_entity(lua_State* state) {
    EntityManager* manager = static_cast<EntityManager*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    LuaManager* luamanager = static_cast<LuaManager*>(lua_tolightuserdata(state,lua_upvalueindex(2)));

    if (!manager || !luamanager) {
        std::cout << "LuaManager or entity manager is null in entmanager add entity";
        return 0;
    }

    int id = lua_tointeger(state,-1);
    lua_pushboolean(state,manager->has_entity(id));
    return 1;
}
