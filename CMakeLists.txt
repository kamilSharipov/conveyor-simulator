cmake_minimum_required(VERSION 3.14)
project(workshop_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sim_core
    lib/item.cpp
    lib/machine.cpp
    lib/workshop.cpp
    lib/parser.cpp
    lib/logger.cpp
)
target_include_directories(sim_core PUBLIC include)

add_executable(simulator src/main.cpp)
target_link_libraries(simulator PRIVATE sim_core)

option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
