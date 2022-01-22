cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(scacchiera VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

include_directories(include)

add_library(chess STATIC
    src/controller.cpp
    src/board.cpp
    src/player.cpp
    src/coordinate.cpp 
    src/movement.cpp 
    src/utilities.cpp
    src/piece.cpp
    src/pieces/king.cpp
    src/pieces/queen.cpp
    src/pieces/bishop.cpp
    src/pieces/knight.cpp
    src/pieces/rook.cpp
    src/pieces/pawn.cpp
)

add_executable(scacchiera 
    src/targets/scacchiera.cpp
)

add_executable(replay 
    src/targets/replay.cpp
)

target_link_libraries(scacchiera PUBLIC chess)
target_link_libraries(replay PUBLIC chess)