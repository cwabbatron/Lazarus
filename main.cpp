#include <iostream>

#include "LuaManager.h"

#include "EntityManager.h"
#include "Entity.h"

#include "nameComponent.h"
#include "TestComponent2.h"
#include <thread>
#include <chrono>
int main(int, char**) {
    using namespace std::chrono_literals;
    LuaManager luamanager;
    
    EntityManager entmanager;

    auto* ent = entmanager.add_entity();
    //TODO: add .add_component .remove_component etc methods
    //TODO: add EntityManager keyword to scripts
    ent->add_lua_component("/home/arthur/projs/Lazarus/testComponent.luau");
    
    
    while (true) {
        ent->update(luamanager);
        
        std::this_thread::sleep_for(1000ms);
    }
    std::cout << "Test, compiled on vscode";
}
