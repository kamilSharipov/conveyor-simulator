cmake_minimum_required(VERSION 3.14)
project(conveyor_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(conv_core
    lib/item.cpp
    lib/machine.cpp
    lib/conveyor.cpp
    lib/parser.cpp
    lib/logger.cpp
)
target_include_directories(conv_core PUBLIC include)

add_executable(simulator src/main.cpp)
target_link_libraries(simulator PRIVATE conv_core)

add_compile_options(-Wall -Wextra -Wpedantic)

add_compile_options(
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:Release>:-DNDEBUG>
    $<$<CONFIG:Release>:-march=native>
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-Werror)
endif()

option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
