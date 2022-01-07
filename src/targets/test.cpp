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

    Board board{"rnbqkbnr/8/8/8/8/8/8/RNBQKBNR", player_1, player_2};
    std::cout << board << "\n\n";

    std::cout << player_1 << "\n\n"
              << player_2 << '\n';
    return 0;
}