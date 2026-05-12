cmake_minimum_required(VERSION 3.14)
project(conveyor_tests)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(run_tests
    test_main.cpp
    test_parser.cpp
    test_machine.cpp
    test_logger.cpp
    test_conveyor.cpp
)

target_link_libraries(run_tests
    conv_core
    gtest_main
    pthread
)

add_test(NAME ConveyorTests COMMAND run_tests)
