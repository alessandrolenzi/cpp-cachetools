cmake_minimum_required(VERSION 3.17)
project(cpp-cachetools)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

add_executable(fibonacci_decorator examples/fibonacci_decorator.cpp)
add_executable(fibonacci_explicit examples/fibonacci_explicit.cpp)
add_executable(function_wrap examples/function_wrap.cpp)