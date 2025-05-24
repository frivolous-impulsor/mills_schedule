cmake_minimum_required(VERSION 3.20.0)

project(Scheduling VERSION 1.0.0 LANGUAGES C CXX)

include(CTest)

enable_testing()

add_executable(Scheduling tests/catch_amalgamated.cpp tests/shiftTest.cpp)

set_property(TARGET Scheduling PROPERTY CXX_STANDARD 17)