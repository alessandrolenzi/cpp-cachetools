cmake_minimum_required(VERSION 3.17)
project(cpp-cachetools)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message("Rebuilding from found ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
else()
    execute_process(
            COMMAND bash -c "conan install .. --remote=conancenter --build=all"
            OUTPUT_VARIABLE CONAN_OUT
    )
    message("Conan install output is ${CONAN_OUT}")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(fibonacci_decorator examples/fibonacci_decorator.cpp)
add_executable(fibonacci_explicit examples/fibonacci_explicit.cpp)
add_executable(function_wrap examples/function_wrap.cpp)