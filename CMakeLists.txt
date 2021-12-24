cmake_minimum_required(VERSION 3.1)

project(scacchiera)

include_directories(include)

add_library(chess STATIC
    src/board.cpp
    src/coordinate.cpp 
    src/movement.cpp 
    src/pieces/king_piece.cpp
)

add_executable(test 
    src/targets/test.cpp
)

target_link_libraries(test PUBLIC chess)