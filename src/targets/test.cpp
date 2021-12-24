#include <iostream>

#include "board.h"

int main() {
    Chess::Board board {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};
    std::cout << board;
    return 0;
}