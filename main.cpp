#include <iostream>

#include "LuaManager.h"
#include "Entity.h"
#include "testcomp.h"
#include "testcomp2.h"
#include <thread>
#include <chrono>
int func(lua_State* state) {
    std::cout << "hello im verity your personal helper friend";
    return 0;
}
//TODO: make everything else
int main(int, char**) {
    using namespace std::chrono_literals;
    LuaManager luamanager;
    
    luamanager.push_function(func,"test_func_from_c");
    luamanager.execute_file("C:/Users/Arthur/Desktop/Lazarus/file.luau");
    Entity ent;

    ent.add_component<TestComp>();
    ent.add_component<TestCompTwo>();
    
    while (true) {
        ent.update();
        std::this_thread::sleep_for(1000ms);
    }
    std::cout << "Test, compiled on vscode";
}
