cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(LosGame)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(VCPKG_TARGET_TRIPLET x64-windows)
set(nlohmann_json_DIR "D:/LosAngelous/vcpkg/installed/x64-windows/share/nlohmann_json")

find_package(nlohmann_json CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(sol2 CONFIG REQUIRED)

file(GLOB_RECURSE LUA_SCRIPTS
    ${CMAKE_SOURCE_DIR}/scripts/*.lua
)
set(SRC_FILE 
    src/main.cc
    
    include/common/constant.h
    include/common/utities.h
    include/common/types.h

    include/core/pool.h
    include/core/engine.h
    include/core/system.h
    include/core/world.h
    include/core/register.h
    include/core/lua_manager.h

    include/entities/combat.h
    include/entities/basic.h
    include/entities/body.h
    include/entities/item.h
    include/entities/map.h
    include/entities/cmd.h
    include/entities/world_time.h

    include/factories/item_factory.h

    src/core/engine.cc
    src/core/register.cc
    src/core/system.cc
    src/core/world.cc
    src/core/lua_manager.cc
    src/entities/item.cc
    src/factories/item_factory.cc
)

add_executable(
    LosGame 
    ${SRC_FILE}
    ${LUA_SCRIPTS} 
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_FILE})
source_group(TREE ${CMAKE_SOURCE_DIR}/scripts
    PREFIX "Scripts"
    FILES ${LUA_SCRIPTS}
)
add_compile_options("/utf-8")
target_link_libraries(
    LosGame 
    PRIVATE 
    nlohmann_json::nlohmann_json
    ${LUA_LIBRARIES}
    sol2::sol2
)

target_include_directories(LosGame 
    PRIVATE 
    "${LUA_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "D:/LosAngelous/vcpkg/installed/x64-windows/include"
)

file(COPY ${CMAKE_SOURCE_DIR}/scripts DESTINATION ${CMAKE_BINARY_DIR})