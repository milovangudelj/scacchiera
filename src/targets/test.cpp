#include <iostream>

#include "Board.h"
#include "Player.h"

int main() {
    //TODO waiting for constructor of Player
    //Player white {}, black;
    Chess::Board board {"rnbqkbnr/8/8/8/8/8/8/RNBQKBNR"};
    std::cout << board;
    return 0;
}