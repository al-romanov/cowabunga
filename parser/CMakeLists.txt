cmake_minimum_required(VERSION 3.13)

project(parser)

include_directories(../common/include)
include_directories(include)

add_library(parser INTERFACE)
target_include_directories(parser INTERFACE include)

target_compile_features(parser INTERFACE cxx_std_17)
