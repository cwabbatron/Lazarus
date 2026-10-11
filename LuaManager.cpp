#include "LuaManager.h"

#include <sstream>
#include <fstream>
#include <iostream>
void LuaManager::execute_string(const std::string &code, const std::string &chunkname)
{
    define_globals();
    const char* csource = code.c_str();

   
    size_t bytecodeSize = 0;
    char* bytecode = luau_compile(csource,strlen(csource),NULL,&bytecodeSize);

    int loadfail = luau_load(state,"chunk",bytecode,bytecodeSize,0);
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

    execute_string(content_string);
}


void LuaManager::push_function(int (*func)(lua_State *),const std::string& name)
{
    lua_pushcfunction(state,func,name.c_str());
    lua_setglobal(state,name.c_str());
}

void LuaManager::execute_lua_update_on_entity(Entity &entity, const std::string &file)
{
    
    execute_file(file);
    lua_getglobal(state,"component");
    if (lua_istable(state,-1)) {
        

        
        lua_getfield(state,-1,"on_update");

        if (lua_isfunction(state,-1)) {
            // if we already have the previous table in the stack, use it!!
            if (lua_istable(state,1)) {
                lua_pushvalue(state,1);
            } else {
                lua_pushvalue(state,-2);
            }
            

            auto returnvalue = lua_pcall(state,1,0,0);

            
            if (returnvalue != LUA_OK) {
                std::cout << "RUNTIME ERROR!! " << lua_tostring(state,-1) << std::endl;
            }
            lua_pop(state,-2);
        } else {
            std::cout << "RUNTIME ERROR!! on_update defined in " << file << " is not a function." << std::endl;
        }

        
    } else {
        std::cout << "RUNTIME ERROR!! component that was returned at " << file << " isnt a table." << std::endl;
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

void LuaManager::define_types()
{
    Entity* ent = static_cast<Entity*>(lua_newuserdata(state,sizeof(Entity)));


}
