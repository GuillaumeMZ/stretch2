cmake_minimum_required(VERSION 3.31)
project(stretch2)

set(CMAKE_CXX_STANDARD 23)

add_executable(stretch2
    src/main.cpp
    src/tokenizer.cpp
)

target_include_directories(stretch2 PRIVATE include)
