cmake_minimum_required(VERSION 3.10)
project(itmo_vcs)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/command)
include_directories(src/stash)
include_directories(src/vcs)

add_executable(itmo_vcs
    src/command/command.cpp
    src/command/command.hpp
    src/command/command_args.cpp
    src/command/command_args.hpp
    src/stash/stash.cpp
    src/stash/stash.hpp
    src/vcs/repository.hpp
    src/main.cpp
        src/logger/logger.hpp
        src/vcs/repository.cpp)
