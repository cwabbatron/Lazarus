#pragma once

#include <unordered_map>
#include <string>
#include <any>
#include <optional>


class Entity;
class Component {
    public:
        virtual ~Component() = default;
        Component(Entity* attachment) : m_attachment(attachment) {};
        Component() : m_attachment(nullptr) {};
        virtual void update(Entity& attachment) = 0;

        virtual void on_attach(Entity& attachment) {};
        virtual void on_deattach(Entity& attachment) {};

        template <typename T>
        void set_property(const std::string& name,T value) {
            m_properties[name] = value;
        }

        template <typename T>
        T* get_property(const std::string& name) {
            auto it = m_properties.find(name);
            if (it == m_properties.end())
                return nullptr;

            return std::any_cast<T>(&it->second);
        }

        template <typename T>
        const T* get_property(const std::string& name) const {
            auto it = m_properties.find(name);
            if (it == m_properties.end())
                return nullptr;

            return std::any_cast<T>(&it->second);
        }

        virtual const std::string get_name() const {
            
            return "abstractComponent";
        };
    private:
        std::unordered_map<std::string,std::any> m_properties;
        Entity* m_attachment = nullptr;
        
};