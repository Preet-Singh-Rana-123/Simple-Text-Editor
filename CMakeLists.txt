cmake_minimum_required(VERSION 3.31)
project(Text_editor)

set(CMAKE_CXX_STANDARD 14)

add_executable(Text_editor
        src/main.cpp
        includes/textEditor.cpp
        includes/textEditor.h)
