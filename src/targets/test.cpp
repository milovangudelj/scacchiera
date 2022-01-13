#include <iostream>
#include <cstring>

#include "Controller.h"
#include "Board.h"
#include "Player.h"

using Chess::Board;
using Chess::Controller;
using Chess::Coordinate;
using Chess::Movement;
using Chess::Player;
using Chess::utilities::Color;
using Chess::utilities::PlayerType;

int main(int argc, char *argv[])
{
    const char *BRIGHT = "\033[1m";
    const char *RESET = "\033[0m";

    if (argc > 1 && strcmp(argv[1], "pc") != 0 && strcmp(argv[1], "cc") != 0)
    {
        std::cout << "Se fornito, il primo argomento deve essere uno tra i seguenti:\n"
                  << BRIGHT << "   pc" << RESET << "   Player vs Computer\n"
                  << BRIGHT << "   cc" << RESET << "   Computer vs Computer\n";
        return 0;
    }

    std::string mode = argc > 1 ? argv[1] : "pc";

    Controller controller{mode};

    controller.play();

    return 0;
}