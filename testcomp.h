#pragma once

#include "Component.h"

#include <iostream>
class TestComp: public Component {
    public:

        void update(Entity& attachment) override {
            std::cout << "yo \n";
        };

        void print_addr_test(Entity& attachment) {
            std::cout << "i am at " << this;
        }
    private:
};