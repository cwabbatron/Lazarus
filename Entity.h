#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include <functional>

#include "Component.h"

#include <memory>
#include <type_traits>
#define MAX_COMPONENTS 128
#define T_IS_COMPONENT(T) std::is_base_of_v<Component,T>


// MASSIVE TODO: make lua components be able to erase and add each other
//for example you cannot erase a lua component

class LuaManager;

class Entity {
    public:
        Entity() {
            m_id = -1;
        }
        Entity(unsigned int id) {
            m_id = id;
        }

        int get_id() const { return m_id; };

        #pragma region normal functions
        //build a new component of type T into m_components
        template <class T>
        T* add_component() {
            
            static_assert(T_IS_COMPONENT(T), "T must be a Component");
            if (has_component<T>()) {
                return nullptr;
            }

            if (m_components.size() + 1 > MAX_COMPONENTS) {
                return nullptr;
            }
            std::unique_ptr<T> comp = std::make_unique<T>(this);
            comp->on_attach(*this);
            T* ptr = comp.get();
            m_components.push_back(std::move(comp));
            return ptr;
        }

        //NOTE: this will invalidate the pointer (pointer will become nullptr) passed since this function moves it to the vector!
        void add_component_unique_ptr(std::unique_ptr<Component> component) {
            component->on_attach(*this);
            m_components.push_back(std::move(component));
        }

        

        
        
        template <class T>
        void remove_component() {
            static_assert(T_IS_COMPONENT(T), "T must be a Component");

            if (!has_component<T>()) {
                return;
            }
            
            m_components.erase(
                std::remove_if(
                    m_components.begin(),
                    m_components.end(),
                    [this](const std::unique_ptr<Component>& comp){
                        const bool should_remove = dynamic_cast<const T*>(comp.get()) != nullptr;
                        if (should_remove) {
                            comp->on_deattach(*this);
                        }
                        return should_remove;
                    }
                ),
                m_components.end()
            );
        };


        
        
        template <class T>
        bool has_component() {
            static_assert(T_IS_COMPONENT(T), "T must be a Component");
            std::vector<std::unique_ptr<Component>>::iterator it = std::find_if(
                m_components.begin(),
                m_components.end(),
                [](const std::unique_ptr<Component>& comp) {
                    return dynamic_cast<const T*>(comp.get()) != nullptr;
                }
            );

            return it != m_components.end() ? true : false;
        };
        #pragma endregion

        
        #pragma region Lua component functions
        
        void add_lua_component(const std::string& file) {
            if (!has_lua_component(file)) {
                m_lua_components.push_back(file);
            }
        }

        void add_lua_component(const std::string& file,std::vector<std::string>& vec) {
            if (!has_lua_component(file,vec)) {
                vec.push_back(file);
            }
        }

        bool has_lua_component(const std::string& file) {
            std::vector<std::string>::iterator it = std::find_if(
                m_lua_components.begin(),
                m_lua_components.end(),
                [file](const std::string& component_file){
                    return component_file == file;
                }
            );

            return it != m_lua_components.end() ? true : false;
        }

        bool has_lua_component(const std::string& file,std::vector<std::string>& vec) {
            std::vector<std::string>::iterator it = std::find_if(
                vec.begin(),
                vec.end(),
                [file](const std::string& component_file){
                    return component_file == file;
                }
            );

            return it != vec.end() ? true : false;
        }
        
        void remove_lua_component(const std::string& file) {
            if (!has_lua_component(file)) { return; };

            m_lua_components.erase(
                std::remove_if(
                    m_lua_components.begin(),
                    m_lua_components.end(),
                    [file](const std::string& lua_component_file){
                        return file == lua_component_file;
                    }
                ),
                m_lua_components.end()
            );
        }

        void remove_lua_component(const std::string& file,std::vector<std::string>& vec) {
            if (!has_lua_component(file,vec)) { return; };

            vec.erase(
                std::remove_if(
                    vec.begin(),
                    vec.end(),
                    [file](const std::string& lua_component_file){
                        return file == lua_component_file;
                    }
                ),
                vec.end()
            );
            
            
        }
        #pragma endregion
        

        #pragma region Component functions that in take in name argument

        void remove_component_name(const std::string& name);

        void add_component_name(std::unordered_map<std::string,std::function<std::unique_ptr<Component>()>>& registry, const std::string& name);

        bool has_component_name(const std::string& name) {
            std::vector<std::unique_ptr<Component>>::iterator it = std::find_if(
                m_components.begin(),
                m_components.end(),
                [name](const std::unique_ptr<Component>& comp) {
                    return comp->get_name() == name;
                }
            );

            return it != m_components.end() ? true : false;
        }
        #pragma endregion
        
        void update(LuaManager& luamanager);
    private:
        std::vector<std::unique_ptr<Component>> m_components;
        std::vector<std::string> m_lua_components;
        int m_id;
};