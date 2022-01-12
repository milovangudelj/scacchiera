#include <iostream>

#include "Board.h"
#include "Player.h"

using Chess::Board;
using Chess::Player;
using Chess::utilities::Color;
using Chess::utilities::PlayerType;

int main()
{
    Player player_1(Color::white, PlayerType::human, "Milo");
    Player player_2(Color::black, PlayerType::computer);

    Board board{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", player_1, player_2};
    std::cout << board << "\n\n";

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
    return 0;
}