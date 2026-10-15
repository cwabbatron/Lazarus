#include <iostream>

#include "LuaManager.h"
#include "Entity.h"
#include "testcomp.h"
#include "testcomp2.h"
#include <thread>
#include <chrono>
int func(lua_State* state) {
    std::cout << "test";
    return 0;
}
//TODO: make everything else
int main(int, char**) {
    using namespace std::chrono_literals;
    LuaManager luamanager;
    
    
    Entity ent(123910293);
    
    luamanager.execute_lua_update_on_entity(ent,"/home/arthur/projs/Lazarus/testComponent.luau");

    luamanager.execute_lua_update_on_entity(ent,"/home/arthur/projs/Lazarus/testComponent.luau");

    luamanager.execute_lua_update_on_entity(ent,"/home/arthur/projs/Lazarus/testComponent2.luau");

    luamanager.execute_lua_update_on_entity(ent,"/home/arthur/projs/Lazarus/testComponent2.luau");
    while (true) {
        ent.update();
        std::cout << "loop running";
        std::this_thread::sleep_for(1000ms);
    }
    std::cout << "Test, compiled on vscode";
}
