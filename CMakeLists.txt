cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project (TicTacToe-Ai)

set( project_sources
   src/main.cpp
   include/game.h
   src/game.cpp
   include/ui.h
   src/ui.cpp
)

add_executable(TicTacToe-Ai ${project_sources})
set_property(TARGET TicTacToe-Ai PROPERTY CXX_STANDARD 11)
set_property(TARGET TicTacToe-Ai PROPERTY CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/pdc34dllw)
target_link_libraries(TicTacToe-Ai ${CMAKE_CURRENT_SOURCE_DIR}/external/pdc34dllw/pdcurses.lib)
