cmake_minimum_required(VERSION 3.13)

project(lexer)

file(GLOB SRCS "lib/*.cpp")

include_directories(../common/include)
include_directories(include lib)
add_library(lexer ${SRCS})

target_compile_features(lexer PUBLIC cxx_std_17)
