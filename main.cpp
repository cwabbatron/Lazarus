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
    
    luamanager.execute_lua_update_on_entity(ent,"C:/Users/Arthur/Desktop/Lazarus/testComponent.luau");

    luamanager.execute_lua_update_on_entity(ent,"C:/Users/Arthur/Desktop/Lazarus/testComponent.luau");

    while (true) {
        ent.update();
        std::this_thread::sleep_for(1000ms);
    }
    std::cout << "Test, compiled on vscode";
}
