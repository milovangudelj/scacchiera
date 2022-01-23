/**
 * @file scacchiera.cpp
 * @author Milovan Gudelj - Mat. 1192574
 * @brief Implementazione dell'eseguibile principale del progetto.
 * @date 2022-22-01
*/

#include <iostream>
#include <cstring>
#include <csignal>

#include "Controller.h"
#include "Board.h"
#include "Player.h"

using namespace Chess;
using namespace Chess::utilities;

std::pair<std::string, std::string> parse_args(int argc, char *argv[]);
void exiting() { std::cout << "\n\033[J"; };

int main(int argc, char *argv[])
{
	// Output cleanup function to be executed upon termination of the process.
	std::atexit(exiting);
	std::signal(SIGINT, exit);
	std::signal(SIGSEGV, exit);

	std::pair<std::string, std::string> params = parse_args(argc, argv);

	std::string mode = params.first; // pc, cc or pp
	std::string fen = params.second; // Forsyth–Edwards Notation (FEN) string representing the initial state of the board

	std::string fen_castling = "r3kbnr/ppp1pppp/2nqb3/3p4/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"; // The white king should be able to castle on queen's side
	std::string fen_checkmate = "1r3k2/6br/n3Q3/pp2Bp1p/2p4P/2N1P3/PPP2PR1/2K5 w - - 0 0";		 // Moving `E5 D6` should cause checkmate for black

	Controller controller{mode, fen};

	controller.play();

	return 0;
}

/**
 * @brief Prints a help message to command line
 */
void print_help()
{
	std::cout << "Se fornito, il primo argomento deve essere uno tra i seguenti:\n\n"
				 << BRIGHT << "	pc" << RESET << "		Player vs Computer\n"
				 << BRIGHT << "	pp" << RESET << "		Player vs Player\n"
				 << BRIGHT << "	cc" << RESET << "		Computer vs Computer\n"
				 << BRIGHT << "	help" << RESET << "		Stampa questo messaggio\n\n"
				 << "Il secondo argomento deve essere una stringa in notazione "
				 << BRIGHT << "FEN" << RESET << ".\nOgni argomento sucessivo al secondo verrà ignorato.\n";
	std::exit(0);
}

/**
 * @brief Parsing of command line parameters given to the executable.
 *        Since the program only needs two parameters if given, it returns an std::pair
 *        containing them if they are valid.
 */
std::pair<std::string, std::string> parse_args(int argc, char *argv[])
{
	std::string def_mode = "pc";																		 // Default game mode. (Player vs Computer)
	std::string def_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"; // Default fen string. Base board configuration.

	std::pair<std::string, std::string> params;

	switch (argc)
	{
	case 1:
		params = std::pair<std::string, std::string>(def_mode, def_fen);
		break;
	case 2:
		if (strcmp(argv[1], "help") == 0 || (strcmp(argv[1], "pc") != 0 && strcmp(argv[1], "pp") != 0 && strcmp(argv[1], "cc") != 0))
		{
			print_help();
		}

		params = std::pair<std::string, std::string>(argv[1], def_fen);
		break;
	default:
		if (strcmp(argv[1], "help") == 0 || (strcmp(argv[1], "pc") != 0 && strcmp(argv[1], "pp") != 0 && strcmp(argv[1], "cc") != 0))
		{
			print_help();
		}

		params = std::pair<std::string, std::string>(argv[1], argv[2]);
		break;
	}

	return params;
}