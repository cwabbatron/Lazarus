#include "LuaManager.h"

#include <sstream>
#include <fstream>
void LuaManager::execute_string(const std::string &code, const std::string &chunkname)
{
    const char* csource = code.c_str();

   
    size_t bytecodeSize = 0;
    char* bytecode = luau_compile(csource,strlen(csource),NULL,&bytecodeSize);

    int result = luau_load(state,"chunk",bytecode,bytecodeSize,0);
    free(bytecode);
    
    lua_pcall(state,0,LUA_MULTRET,0);
}

void LuaManager::execute_file(const std::string &directory, const std::string &chunkname)
{
    std::fstream file(directory);
    
    std::stringstream content_stream;
    content_stream << file.rdbuf();

    std::string content_string = content_stream.str();

    execute_string(content_string);
}
