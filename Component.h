#pragma once

#include <unordered_map>
#include <string>
#include <any>
#include <optional>
class Component {
    public:
        ~Component() = default;
        virtual void update() = 0;


       //MASSIVE TODO: implement properties
    private:
};