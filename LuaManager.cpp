#include "LuaManager.h"

#include "Entity.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cstring>

#include "nameComponent.h"
#include "TestComponent2.h"

#include "LuaMethodDefinitions.h"
#define COMPONENT_REGISTER(NAME) m_component_registry[#NAME] = []{ return std::make_unique<NAME>(); };

#pragma region Lua Execution

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
#pragma endregion

#pragma region Definitions

int print(lua_State* state) {
    auto parameter = lua_tostring(state,-1);

    std::cout << "\e[0;94m[LUA]: " << parameter << "\e[0m" << std::endl;
    return 0;
}
void LuaManager::define_globals()
{
    lua_pushcfunction(state,print,"print");
    lua_setglobal(state,"print");
   
}

void LuaManager::fill_registry()
{
    COMPONENT_REGISTER(NameComponent);
    COMPONENT_REGISTER(TestComponentTwo);

    add_to_component_name_registry("SecondaryComponent");

}

#pragma endregion

#pragma region Method attachment and entity conversion

void LuaManager::attach_methods_to_self(const std::string& name,Entity& entity)
{
   int ref_index = m_component_table_references.at(name);
   
   int returntype = lua_getref(state,ref_index); // [comp]

   if (returntype == LUA_TNIL || returntype == LUA_REFNIL) {
    std::cout << "INVALID REFERENCE INDEX!!!!!!!!!!!!!" << std::endl;
        return;
   }

   

   
   lua_pushlightuserdata(state,&entity);
   lua_pushlightuserdata(state,this);
   lua_pushcclosure(state,get_attachment_lua,"get_attachment",2); // [comp, ccls]

   lua_setfield(state,-2,"get_attachment"); // [comp]


}


void LuaManager::convert_entity_to_table(Entity &entity)
{
    lua_newtable(state); // [ent]

    lua_pushlightuserdata(state,&entity); //[ent, userd]
    lua_pushcclosure(state,get_id_lua,"get_id",1); //[ent, ccls]
    

    lua_setfield(state,-2,"get_id"); //[ent]
    #pragma region cpp component methods

    lua_pushlightuserdata(state,&entity);
    lua_pushcclosure(state,has_component_lua_cpp,"has_component_cpp",1);

    lua_setfield(state,-2,"has_component_cpp"); //[ent]

    lua_pushlightuserdata(state,&entity);
    lua_pushlightuserdata(state,this);
    lua_pushcclosure(state,add_component_lua_cpp,"add_component_cpp",2);

    lua_setfield(state,-2,"add_component_cpp");
    
    lua_pushlightuserdata(state,&entity);
    lua_pushcclosure(state,remove_component_lua_cpp,"remove_component_cpp",1);

    lua_setfield(state,-2,"remove_component_cpp");

    #pragma endregion

    #pragma region lua component methods

    lua_pushlightuserdata(state,&entity);
    lua_pushlightuserdata(state,this);

    lua_pushcclosure(state,has_component_lua_lua,"has_component_lua_lua",2);

    lua_setfield(state,-2,"has_component_lua");

    lua_pushlightuserdata(state,&entity);
    lua_pushlightuserdata(state,this);

    lua_pushcclosure(state,add_component_lua_lua,"add_component_lua_lua",2);

    lua_setfield(state,-2,"add_component_lua");

    lua_pushlightuserdata(state,&entity);
    lua_pushlightuserdata(state,this);

    lua_pushcclosure(state,remove_component_lua_lua,"remove_component_lua_lua",2);

    lua_setfield(state,-2,"remove_component_lua");
    
    #pragma endregion
}

#pragma endregion

#pragma region other functions
std::function<std::unique_ptr<Component>()> LuaManager::get_component_from_registry(const std::string &component_name)
{
    
    return m_component_registry.at(component_name);
}

std::string LuaManager::get_file_from_name_registry(const std::string &name)
{
    try {
        return m_component_name_registry.at(name);
    } catch (const std::out_of_range& error) {
        std::cout << "Error in get file from name in registry, error: " << error.what();

        return "";
    }
    
}
#pragma endregion