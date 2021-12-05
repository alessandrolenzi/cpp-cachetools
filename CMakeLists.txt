cmake_minimum_required(VERSION 3.17)
project(cpp-cachetools)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message("Rebuilding from found ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
else()
    execute_process(
            COMMAND bash -c "conan install .. --remote=conancenter --build"
            OUTPUT_VARIABLE CONAN_OUT
    )
    message("Conan install output is ${CONAN_OUT}")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(fibonacci_decorator examples/fibonacci_decorator.cpp)
add_executable(fibonacci_explicit examples/fibonacci_explicit.cpp)
add_executable(function_wrap examples/function_wrap.cpp)
message("Source dir is  ${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR})
enable_testing()

add_executable(
        tests
        tests/lru_cache_wrap/test_static.cpp
        tests/lru_cache_wrap/test_lambda.cpp
        tests/lru_cache_wrap/test_free_function.cpp
        tests/lru_cache_wrap/test_member_function.cpp
        tests/lru_cache/test_lru_cache.cpp
)

target_include_directories(
        tests
        PUBLIC
        ${CMAKE_SOURCE_DIR}
)
target_link_libraries(
        tests
        gtest
        gtest_main
        pthread
)
include(GoogleTest)
gtest_discover_tests(tests)