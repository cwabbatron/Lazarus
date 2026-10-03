#include <iostream>

#include "LuaManager.h"
#include "Entity.h"
#include "testcomp.h"
//TODO: make everything else
int main(int, char**) {
    LuaManager luamanager;
    Entity ent;

    ent.add_component<TestComp>();

    
    luamanager.execute_file("./file.luau");
    while (true) {
        ent.update();
    }
    std::cout << "Test, compiled on vscode";
}
