cmake_minimum_required(VERSION 3.29.0)
project(build)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wpedantic -Werror)

add_executable(build main.cpp Container.h Object.h)