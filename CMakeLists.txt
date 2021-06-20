cmake_minimum_required(VERSION 3.5)

project(matrix)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(matrix main.cpp matrix.h)
add_executable(tests tests.cpp matrix.h)

set_target_properties(matrix tests PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
