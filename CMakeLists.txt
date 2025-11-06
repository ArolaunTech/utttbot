cmake_minimum_required(VERSION 3.25)

project (
    uttt
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE uttt_src
    "src/*.h"
    "src/*.cpp"
)

add_executable(utttbot ${uttt_src})