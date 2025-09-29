cmake_minimum_required(VERSION 3.10)
project(compiler_test)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#debug
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.c")

add_executable(main ${SOURCES})
