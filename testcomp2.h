#pragma once

#include "Component.h"

#include <iostream>
class TestCompTwo: public Component {
    public:

        void update() override {
            std::cout << "yo 2 \n";
        };
    private:
};