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
    std::string fen = argc > 2 ? argv[2] : "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";

    std::string fen_castling = "r3kbnr/ppp1pppp/2nqb3/3p4/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
    std::string fen_checkmate = "1r3k2/6br/n3Q3/pp2Bp1p/2p4P/2N1P3/PPP2PR1/2K5 w - - 0 0"; // Moving `E5 D6` should cause checkmate for black

    Controller controller{mode, fen};

    controller.play();

    return 0;
}