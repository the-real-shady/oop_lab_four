cmake_minimum_required(VERSION 3.16)

project(oop_lab_four LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

set(PROJECT_WARNING_FLAGS
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
)

add_executable(oop_lab_four
    src/main.cpp
)

target_include_directories(oop_lab_four PRIVATE include)

target_compile_options(oop_lab_four PRIVATE ${PROJECT_WARNING_FLAGS})

if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(oop_lab_four_tests
        tests/test_figures.cpp
    )

    target_include_directories(oop_lab_four_tests PRIVATE include)
    target_link_libraries(oop_lab_four_tests PRIVATE gtest_main)
    target_compile_options(oop_lab_four_tests PRIVATE ${PROJECT_WARNING_FLAGS})

    add_test(NAME oop_lab_four_tests COMMAND oop_lab_four_tests)
endif()
