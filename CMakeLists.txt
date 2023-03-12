cmake_minimum_required(VERSION 3.13)

project(kaleidoc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(common)
include_directories(common/include)

add_subdirectory(lexer)
include_directories(lexer/include)

add_subdirectory(parser)
include_directories(parser/include)

file(GLOB DRIVER_SRCS "cbc/*.cpp")
include_directories(cbc)
add_executable(cbc ${DRIVER_SRCS})

target_link_libraries(cbc common lexer parser)

target_compile_features(cbc PUBLIC cxx_std_17)
