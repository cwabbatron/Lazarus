#pragma once


#include "lua.h"
#include "luacode.h"
#include "lualib.h"
#include <string>

#include <functional>
class LuaManager {
    public:
        LuaManager() {
            state = luaL_newstate();
            luaL_openlibs(state);
        }


        void execute_string(const std::string& code,const std::string& chunkname = "chunk");

        void execute_file(const std::string& directory, const std::string& chunkname = "chunk");

        void push_function(int (*func)(lua_State*),const std::string& name);
    private:
        lua_State* state = nullptr;
};