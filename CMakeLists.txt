cmake_minimum_required(VERSION 3.10.2)
project(stash VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

file(GLOB SOURCES
        "src/command/*.cpp"
        "src/stash/*.cpp"
        "src/vcs/*.cpp"
        "src/main.cpp"
)

file(GLOB HEADERS
        "include/command/*.hpp"
        "include/exception/*.h"
        "include/logger/*.hpp"
        "include/stash/*.hpp"
        "include/vcs/*.hpp"
)

add_executable(stash ${SOURCES} ${HEADERS})

target_include_directories(stash PRIVATE
        include/command
        include/exception
        include/logger
        include/stash
        include/vcs
)