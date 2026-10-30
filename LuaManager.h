#pragma once


#include "lua.h"
#include "luacode.h"
#include "lualib.h"
#include <string>

#include "Component.h"

#include <functional>
#include <unordered_map>
#include <memory>
class Entity;
class LuaManager {
    public:
        LuaManager() {
            state = luaL_newstate();
            luaL_openlibs(state);

            fill_registry();
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

        void attach_methods_to_self(const std::string& component_name,Entity& entity);

        void convert_entity_to_table(Entity& entity);

        void fill_registry();

        std::function<std::unique_ptr<Component>()> get_component_from_registry(const std::string& component_name);
        std::unordered_map<std::string,std::function<std::unique_ptr<Component>()>>& get_registry() {
            return m_component_registry;
        }
    private:
        
        
        lua_State* state = nullptr;
        std::unordered_map<std::string,int> m_component_table_references;
        std::unordered_map<std::string,std::function<std::unique_ptr<Component>()>> m_component_registry;
};