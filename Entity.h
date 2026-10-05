#pragma once

#include <vector>

#include "Component.h"
#include <memory>
#include <type_traits>
#define MAX_COMPONENTS 128
#define T_IS_COMPONENT(T) std::is_base_of_v<Component,T>
class Entity {
    public:

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
            std::unique_ptr<T> comp = std::make_unique<T>();
            comp->on_attach(*this);
            T* ptr = comp.get();
            m_components.push_back(std::move(comp));
            return ptr;
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
                    [](const std::unique_ptr<Component>& comp){
                        comp->on_deattach();
                        return dynamic_cast<const T*>(comp.get()) != nullptr;
                    }
                )
            )
        };

        void update() {
            for (auto& comp : m_components) {
                comp->update(*this);
            }
        }
    private:
        std::vector<std::unique_ptr<Component>> m_components;

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
        
};