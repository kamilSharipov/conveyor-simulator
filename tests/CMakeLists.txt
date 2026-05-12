cmake_minimum_required(VERSION 3.10)
project(conveyor_tests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/../include)
link_directories(${CMAKE_SOURCE_DIR}/../build)

add_executable(run_tests
    test_main.cpp
    test_parser.cpp
    test_machine.cpp
    test_logger.cpp
    test_conveyor.cpp
)

target_link_libraries(run_tests
    conv_core
    ${GTEST_LIBRARIES}
    pthread
)

add_test(NAME ConveyorTests COMMAND run_tests)