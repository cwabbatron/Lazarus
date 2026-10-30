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

        void add_component_registry(std::unordered_map<std::string,std::function<std::unique_ptr<Component>()>>& registry, const std::string& name);

        //TODO: add checks for existing component and max components
        //and add on_attach function call
        void add_lua_component(const std::string& file) {
            m_lua_components.push_back(file);
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
                        comp->on_deattach(*this);
                        return dynamic_cast<const T*>(comp.get()) != nullptr;
                    }
                )
            );
        };


        void update(LuaManager& luamanager);
        
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
        bool has_lua_component(const std::string& component_name) {
            std::vector<std::string>::iterator it = std::find_if(
                m_lua_components.begin(),
                m_lua_components.end(),
                [component_name](const std::string& component_file){
                    return component_file == component_name;
                }
            );

            return it != m_lua_components.end() ? true : false;
        }
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
    private:
        std::vector<std::unique_ptr<Component>> m_components;
        std::vector<std::string> m_lua_components;
        int m_id;
};