cmake_minimum_required(VERSION 3.13)

project(common)

file(GLOB SRCS "lib/*.cpp")

include_directories(include lib)
add_library(common ${SRCS})

target_compile_features(common PUBLIC cxx_std_17)
