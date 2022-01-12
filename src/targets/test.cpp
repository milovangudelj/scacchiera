#include <iostream>

#include "Controller.h"
#include "Board.h"
#include "Player.h"

using Chess::Board;
using Chess::Controller;
using Chess::Player;
using Chess::utilities::Color;
using Chess::utilities::PlayerType;
using Chess::Movement;
using Chess::Coordinate;

int main()
{
    Controller controller{};

    controller.play();

    /*
    std::cout << player_1 << "\n\n"
              << player_2 << '\n';
    board.move(player_1, player_2, {{6, 3}, {4, 3}});
    std::cout << board << "\n\n";
    board.move(player_1, player_2, {{7, 3}, {5, 3}});
    std::cout << board << "\n\n";
    board.move(player_1, player_2, {{7, 2}, {6, 3}});
    std::cout << board << "\n\n";
    board.move(player_1, player_2, {{7, 1}, {5, 0}});
    std::cout << board << "\n\n";
    board.move(player_1, player_2, {{7, 0}, {7, 4}});
    std::cout << board << "\n\n";
    */
    Coordinate start {6, 3};
    Coordinate end {3, 3};
    Movement movement {start, end};
   std::cout << movement;
    return 0;
}