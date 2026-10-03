#pragma once

#include "Component.h"

#include <iostream>
class TestComp: public Component {
    public:

        void update() override {
            std::cout << "yo \n";
        };
    private:
};