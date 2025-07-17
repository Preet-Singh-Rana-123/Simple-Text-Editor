cmake_minimum_required(VERSION 3.31)
project(Text_editor)

set(CMAKE_CXX_STANDARD 14)

add_executable(Text_editor
        src/main.cpp
        src/TextEditor.cpp
        includes/TextEditor.h
)

find_library(NCURSES_LIB ncurses PATHS /opt/homebrew/opt/ncurses/lib)
include_directories(/opt/homebrew/opt/ncurses/include)

target_link_libraries(Text_editor ${NCURSES_LIB})