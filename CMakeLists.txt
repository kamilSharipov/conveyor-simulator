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

option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
