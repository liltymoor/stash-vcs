cmake_minimum_required(VERSION 3.10)
project(stash)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(src/command)
include_directories(src/stash)
include_directories(src/vcs)

add_executable(stash
    src/command/command.cpp
    src/command/command.hpp
    src/command/command_args.cpp
    src/command/command_args.hpp
    src/stash/stash.cpp
    src/stash/stash.hpp
    src/vcs/repository.hpp
    src/main.cpp
        src/logger/logger.hpp
        src/vcs/repository.cpp
        src/exception/pers_stack_exc.h)
