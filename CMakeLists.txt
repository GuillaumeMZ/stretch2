cmake_minimum_required(VERSION 3.31)
project(stretch2)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(stretch2
    src/main.cpp
    src/tokenizer.cpp
)

target_include_directories(stretch2 PRIVATE include)

add_executable(stretch2_tests
    src/tokenizer.cpp

    tests/test_tokenizer.cpp
)

target_include_directories(stretch2_tests PRIVATE include)

target_link_libraries(stretch2_tests PRIVATE Catch2::Catch2WithMain)