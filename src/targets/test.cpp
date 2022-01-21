#include <iostream>
#include <cstring>
#include <csignal>

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

const char *BRIGHT = "\033[1m";
const char *RESET = "\033[0m";

std::pair<std::string, std::string> parse_args(int argc, char *argv[]);

void exiting()
{
	std::cout << "\n\033[J";
};

int main(int argc, char *argv[])
{
	// Register function to be executed on exit or `ctrl + c`
	std::atexit(exiting);
	std::signal(SIGINT, exit);

	std::pair<std::string, std::string> params = parse_args(argc, argv);

	std::string mode = params.first;
	std::string fen = params.second;

	std::string fen_castling = "r3kbnr/ppp1pppp/2nqb3/3p4/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
	std::string fen_checkmate = "1r3k2/6br/n3Q3/pp2Bp1p/2p4P/2N1P3/PPP2PR1/2K5 w - - 0 0"; // Moving `E5 D6` should cause checkmate for black
	std::string fen_promotion = "5k2/8/8/8/8/5brr/p4bBR/5nRK b - - 0 0";
	Controller controller{mode, fen_promotion};

	controller.play();

	return 0;
}

void print_help()
{
	std::cout << "Se fornito, il primo argomento deve essere uno tra i seguenti:\n\n"
				 << BRIGHT << "	pc" << RESET << "		Player vs Computer\n"
				 << BRIGHT << "	cc" << RESET << "		Computer vs Computer\n"
				 << BRIGHT << "	help" << RESET << "		Stampa questo messaggio\n\n"
				 << "Il secondo argomento deve essere una stringa in notazione "
				 << BRIGHT << "FEN" << RESET << ".\nOgni argomento sucessivo al secondo verrà ignorato.\n\n";
	std::exit(0);
}

std::pair<std::string, std::string> parse_args(int argc, char *argv[])
{
	std::string def_mode = "pc";
	std::string def_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
	std::pair<std::string, std::string> params;

	switch (argc)
	{
	case 1:
		params = std::pair<std::string, std::string>(def_mode, def_fen);
		break;
	case 2:
		if ((strcmp(argv[1], "pc") != 0 && strcmp(argv[1], "cc") != 0) || strcmp(argv[1], "help") == 0)
			print_help();
		params = std::pair<std::string, std::string>(argv[1], def_fen);
		break;
	default:
		if ((strcmp(argv[1], "pc") != 0 && strcmp(argv[1], "cc") != 0) || strcmp(argv[1], "help") == 0)
			print_help();
		params = std::pair<std::string, std::string>(argv[1], argv[2]);
		break;
	}

	return params;
}