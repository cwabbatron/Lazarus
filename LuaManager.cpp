#include "LuaManager.h"

#include "Entity.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>

void LuaManager::execute_string(const std::string &code, const std::string &chunkname)
{
    define_globals();
    const char* csource = code.c_str();

   
    size_t bytecodeSize = 0;
    char* bytecode = luau_compile(csource,strlen(csource),NULL,&bytecodeSize);

    int loadfail = luau_load(state,chunkname.c_str(),bytecode,bytecodeSize,0);
    free(bytecode);
    
    if (loadfail) {
        std::cout << "Error loading bytecode.";
        return;
    }
    int runtime_error = lua_pcall(state,0,LUA_MULTRET,0);

    if (runtime_error != LUA_OK) {
        const char* error_string = lua_tostring(state,-1);

        std::cout << "RUNTIME ERROR EXECUTING " << chunkname << " error: " << error_string << "\n";
    }
}

void LuaManager::execute_file(const std::string &directory, const std::string &chunkname)
{
    std::fstream file(directory);
    
    std::stringstream content_stream;
    content_stream << file.rdbuf();

    std::string content_string = content_stream.str();

    execute_string(content_string, directory);
}


void LuaManager::push_function(int (*func)(lua_State *),const std::string& name)
{
    lua_pushcfunction(state,func,name.c_str());
    lua_setglobal(state,name.c_str());
}

void LuaManager::push_function_table(int (*func)(lua_State *), const std::string &name)
{
    lua_pushcfunction(state,func,name.c_str());
    lua_setfield(state,-1,name.c_str());
}

void LuaManager::execute_lua_update_on_entity(Entity &entity, const std::string &file)
{
    
    execute_file(file);
    lua_getglobal(state,"component");

    if (!lua_istable(state,-1)) {
        std::cout << "RUNTIME ERROR!! component at " << file << " isnt a table.";
        return;
    }
    int returntype = lua_getfield(state,-1,"name");

    if (returntype == LUA_TNIL || returntype != LUA_TSTRING) {
        std::cout << "RUNTIME ERROR!! name is nil or isnt a string. ";
        return;
    }

    const char* name = lua_tostring(state,-1);

    //if DONT have the reference to the component stored, store it.
    if (m_component_table_references.find(name) == m_component_table_references.end()) {
        int ref = lua_ref(state,-2);
        
        m_component_table_references.emplace(name,ref);
        
    } 

    attach_methods_to_self(name,entity);

    int reference_index = m_component_table_references.at(name);

    int reference_type = lua_getref(state,reference_index);

    if (reference_type == LUA_TNIL) {
        std::cout << "RUNTIME ERROR!! reference is nil.";
        return;
    }

    lua_getfield(state,-1,"on_update");

    if (!lua_isfunction(state,-1)) {
        std::cout << "RUNTIME ERROR!! on_update isnt a function. ";
        return;
    }
    
    lua_pushvalue(state,-2);
    
    

    int result = lua_pcall(state,1,0,0);
    if (result != LUA_OK) {
        const char* error = lua_tostring(state,-1);
        std::cout << "RUNTIME ERROR!! " << error << "\n";
        return;
    }
}

void LuaManager::define_globals()
{
    push_function([](lua_State* state) -> int {
        auto parameter = lua_tostring(state,-1);

        std::cout << "[LUA]: " << parameter << std::endl;
        return 0;
    },"print");

   
}
int test(lua_State* state) {
    
    std::cout << "Different test!!! \n"; 
    return 1;
}

//todo finish this
int get_attachment_lua(lua_State* state) {
    Entity* entity = static_cast<Entity*>(lua_tolightuserdata(state,lua_upvalueindex(1)));

    LuaManager* manager = static_cast<LuaManager*>(lua_tolightuserdata(state,lua_upvalueindex(2)));

    manager->convert_entity_to_table(*entity);

    return 1;
}
void LuaManager::attach_methods_to_self(const std::string& name,Entity& entity)
{
    
   int ref_index = m_component_table_references.at(name);
   
   int returntype = lua_getref(state,ref_index); // [comp]

   if (returntype == LUA_TNIL || returntype == LUA_REFNIL) {
    std::cout << "INVALID REFERENCE INDEX!!!!!!!!!!!!!" << std::endl;
        return;
   }

   lua_pushcfunction(state,test,"test");
   lua_setfield(state,-2,"test");

   
   lua_pushlightuserdata(state,&entity);
   lua_pushlightuserdata(state,this);
   lua_pushcclosure(state,get_attachment_lua,"get_attachment",2); // [comp, ccls]

   lua_setfield(state,-2,"get_attachment"); // [comp]


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


void LuaManager::convert_entity_to_table(Entity &entity)
{
    lua_newtable(state); // [ent]

    lua_pushlightuserdata(state,&entity); //[ent, userd]
    lua_pushcclosure(state,get_id_lua,"get_id",1); //[ent, ccls]
    

    lua_setfield(state,-2,"get_id"); //[ent]

    lua_pushlightuserdata(state,&entity);
    lua_pushcclosure(state,has_component_lua,"has_component",1);

    lua_setfield(state,-2,"has_component"); //[ent]

    
}
