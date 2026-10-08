cmake_minimum_required(VERSION 3.10.0)
project(Lazarus VERSION 0.1.0 LANGUAGES C CXX)
add_executable(Lazarus main.cpp LuaManager.cpp)

include(FetchContent)

FetchContent_Declare(Luau
    GIT_REPOSITORY https://github.com/luau-lang/luau.git
    GIT_TAG 0.740
)

FetchContent_MakeAvailable(Luau)

target_link_libraries(Lazarus PRIVATE Luau.VM Luau.Compiler)