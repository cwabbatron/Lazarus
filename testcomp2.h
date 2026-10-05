#pragma once

#include "Component.h"

#include <iostream>
class TestCompTwo: public Component {
    public:

        void update(Entity& attachment) override {
            std::cout << "yo 2 \n";
        };
    private:
};