#include <iostream>

#include "LuaManager.h"

#include "EntityManager.h"
#include "Entity.h"

#include "nameComponent.h"

#include <thread>
#include <chrono>
int main(int, char**) {
    using namespace std::chrono_literals;
    LuaManager luamanager;
    
    EntityManager entmanager;

    auto* ent = entmanager.add_entity();
    ent->add_component<NameComponent>();
    
    luamanager.execute_lua_update_on_entity(*ent,"/home/arthur/projs/Lazarus/testComponent.luau");
    while (true) {
        ent->update();
        std::cout << "loop running" << std::endl;
        std::this_thread::sleep_for(200ms);
    }
    std::cout << "Test, compiled on vscode";
}
