cmake_minimum_required(VERSION 3.0)
project(labyrinth)

set(CMAKE_CXX_STANDARD 11)

add_executable(labyrinth
        main.cpp
        main.hpp
        helper.cpp
        helper.hpp
        global.hpp
        prettyprint.hpp)

if(MSVC)
    target_compile_options(labyrinth PRIVATE /W4 /WX)
else(MSVC)
    target_compile_options(labyrinth PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)