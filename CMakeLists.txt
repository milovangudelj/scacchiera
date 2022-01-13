cmake_minimum_required(VERSION 3.1)

project(scacchiera)

include_directories(include)

add_library(chess STATIC
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

add_executable(test 
    src/targets/test.cpp
)

add_executable(replay 
    src/targets/replay.cpp
)

target_link_libraries(test PUBLIC chess)
target_link_libraries(replay PUBLIC chess)