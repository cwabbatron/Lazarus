#pragma once


#include "lua.h"
#include "luacode.h"
#include "lualib.h"
#include <string>

#include <functional>
#include "Entity.h"
#include <unordered_map>
class LuaManager {
    public:
        LuaManager() {
            state = luaL_newstate();
            luaL_openlibs(state);
        }

        ~LuaManager() {
            for (const auto& reference : m_component_table_references) {
                lua_unref(state,reference.second);
            }

            lua_close(state);
        }

        void execute_string(const std::string& code,const std::string& chunkname = "chunk");

        void execute_file(const std::string& directory, const std::string& chunkname = "chunk");

        void push_function(int (*func)(lua_State*),const std::string& name);

        void push_function_table(int (*func)(lua_State*),const std::string& name);
        
        void execute_lua_update_on_entity(Entity& entity, const std::string& file);

        void define_globals();

        void attach_metatable_to_self(const std::string& component_name);

        void define_types();
    private:
        lua_State* state = nullptr;
        std::unordered_map<std::string,int> m_component_table_references;
};