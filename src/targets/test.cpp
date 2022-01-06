#include <iostream>

#include "Board.h"
#include "Player.h"

using Chess::Board;
using Chess::Player;

int main() {
    Board board{"rnbqkbnr/8/8/8/8/8/8/RNBQKBNR"};
    std::cout << board << "\n\n";

    Player player_1("white", "human", "Milo");
    Player player_2("black", "computer");

    std::cout << player_1 << "\n\n"
              << player_2 << '\n';
    return 0;
}