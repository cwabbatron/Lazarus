#pragma once

#include "Component.h"
#include <iostream>
class NameComponent : public Component {
    public:
        using Component::Component;
        void update(Entity& attachment) override {
            std::cout << "namecomponent update" << std::endl;
        };

        const std::string get_name() const override {
            return "NameComponent";
        }
    private:
};