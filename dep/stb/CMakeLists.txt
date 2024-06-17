cmake_minimum_required(VERSION 3.25)
project(stb)

add_library(stb STATIC stb.c)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
