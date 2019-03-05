cmake_minimum_required(VERSION 3.14)
project(GameBoy-Classic-Emulator)
 
file(GLOB SOURCES 
    "include/*.hpp"
    "src/*.cpp"
)
 
add_executable(GameBoy-Classic-Emulator ${SOURCES})
