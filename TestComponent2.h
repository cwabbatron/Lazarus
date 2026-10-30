#pragma once

#include "Component.h"
#include <iostream>
class TestComponentTwo : public Component {
    public:
        using Component::Component;
        void update(Entity& attachment) override {
            std::cout << "TestComponentTwo update!!!!!!!1" << std::endl;
        };

        const std::string get_name() const override {
            return "TestComponentTwo";
        }
    private:
};