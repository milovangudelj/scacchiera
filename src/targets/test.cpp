#include <iostream>

#include "Board.h"
#include "Player.h"

int main() {
    Chess::Board board {"rnbqkbnr/8/8/8/8/8/8/RNBQKBNR"};
    std::cout << board;
    return 0;
}