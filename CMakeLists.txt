cmake_minimum_required(VERSION 3.14)
project(GameBoy-Classic-Emulator)

find_package(SDL2 REQUIRED)
include_directories(GameBoy-Classic-Emulator ${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES 
    "include/*.hpp"
    "src/*.cpp"
)
 
add_executable(GameBoy-Classic-Emulator ${SOURCES})

target_link_libraries(GameBoy-Classic-Emulator ${SDL2_LIBRARIES})