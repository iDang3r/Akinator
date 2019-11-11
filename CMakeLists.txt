cmake_minimum_required(VERSION 3.14)
project(Akinator)

set(CMAKE_CXX_STANDARD 17)

include_directories(/Users/alex/Desktop/my_libs)

add_executable(Akinator main.cpp Tree.h Akinator_Tree.h color_printf.h)