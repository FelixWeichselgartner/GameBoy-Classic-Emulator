cmake_minimum_required(VERSION 3.10)
project(GameBoy-Classic-Emulator)

find_package(SDL2 REQUIRED)

find_program(LSB_RELEASE_EXEC lsb_release)
execute_process(COMMAND ${LSB_RELEASE_EXEC} -is
    OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message( ${LSB_RELEASE_ID_SHORT} )

if (NOT LSB_RELEASE_ID_SHORT STREQUAL "ManjaroLinux")
    include_directories(GameBoy-Classic-Emulator ${SDL2_INCLUDE_DIRS})
endif()


file(GLOB SOURCES 
    "include/*.hpp"
    "src/*.cpp"
)
 
add_executable(GameBoy-Classic-Emulator ${SOURCES})

if (LSB_RELEASE_ID_SHORT STREQUAL "ManjaroLinux")
    target_link_libraries(GameBoy-Classic-Emulator SDL2::SDL2)
else()
    target_link_libraries(GameBoy-Classic-Emulator ${SDL2_LIBRARIES})
endif()
