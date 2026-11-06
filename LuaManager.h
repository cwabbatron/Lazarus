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
        
        void execute_lua_update_on_entity(Entity& entity, const std::string& file);

        void define_globals();

        void attach_methods_to_self(const std::string& component_name,Entity& entity);

        void convert_entity_to_table(Entity& entity);

        void fill_registry();

        std::function<std::unique_ptr<Component>()> get_component_from_registry(const std::string& component_name);

        std::unordered_map<std::string,std::function<std::unique_ptr<Component>()>>& get_registry() {
            return m_component_registry;
        }

        std::string get_file_from_name_registry(const std::string& name);

        void add_to_component_name_registry(const std::string& name, const std::string& file) {
            m_component_name_registry.insert({name,file});
        };

        void add_to_component_name_registry(const std::string& name) {
            m_component_name_registry.insert({name,std::string("/home/arthur/projs/Lazarus/scripts/") + name + ".luau"});
        }
        
    private:
        
        
        lua_State* state = nullptr;
        std::unordered_map<std::string,int> m_component_table_references;
        std::unordered_map<std::string,std::function<std::unique_ptr<Component>()>> m_component_registry;
        //stores component names to files
        std::unordered_map<std::string,std::string> m_component_name_registry;
};