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

    return 0;
}