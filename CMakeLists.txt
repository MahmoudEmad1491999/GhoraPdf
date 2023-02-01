cmake_minimum_required(VERSION 3.16)

project(GhoraPdf LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(Ghora main.c)
