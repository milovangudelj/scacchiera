#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>

#include "Controller.h"
#include "Board.h"

using Chess::Board;
using Chess::Controller;
using Chess::utilities::Color;
using Chess::utilities::PlayerType;

static const char *BRIGHT = "\033[1m";
static const char *RESET = "\033[0m";
static const char *RED_FG = "\033[31m";
static const char *BLUE_FG = "\033[34m";

Controller::Controller() : white(nullptr), black(nullptr), board(nullptr)
{
	init("pc");
}

void Controller::init(const std::string &type)
{
	if (type.compare("pc") == 0)
	{
		bool white_is_human = rand() % 1 == 1;

		white = std::make_shared<Player>(Color::white, PlayerType{white_is_human ? PlayerType::human : PlayerType::computer});
		black = std::make_shared<Player>(Color::black, PlayerType{white_is_human ? PlayerType::computer : PlayerType::human});
	}
	else if (type.compare("cc") == 0)
	{
		white = std::make_shared<Player>(Color::white, PlayerType::computer);
		black = std::make_shared<Player>(Color::black, PlayerType::computer);
	}
	else
	{
		throw "Invalid game type";
	}

	std::string base_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	std::string fen = "1r3k2/6br/n2BQ3/pp3p1p/2p4P/2N1P3/PPP2PR1/2K5";
	board = std::make_unique<Board>(fen, *white.get(), *black.get());
}

std::map<char, unsigned int> col_mapping{
	 {'A', 0},
	 {'B', 1},
	 {'C', 2},
	 {'D', 3},
	 {'E', 4},
	 {'F', 5},
	 {'G', 6},
	 {'H', 7}};

std::map<char, unsigned int> row_mapping{
	 {'1', 7},
	 {'2', 6},
	 {'3', 5},
	 {'4', 4},
	 {'5', 3},
	 {'6', 2},
	 {'7', 1},
	 {'8', 0}};

Chess::Movement Controller::get_move()
{
	std::string from = "";
	std::string to = "";

	std::cin >> from >> to;
	std::cout << "\033[14A\033[J";

	if (from.compare("XX") == 0 && to.compare("XX") == 0)
	{
		return {{9, 9}, {9, 9}};
	}

	Chess::Coordinate start = {row_mapping[from.at(1)], col_mapping[from.at(0)]};
	Chess::Coordinate end = {row_mapping[to.at(1)], col_mapping[to.at(0)]};

	return {start, end}; // {{yf, xf},{yt, xt}} where f = from, t = to
}

/// @brief A helper function for setting error messages
/// @param errors The reference to the errors set
/// @param message The error message you want to set
/// @returns void
void set_error(std::set<std::string> &errors, const std::string &message)
{
	std::stringstream msg;
	msg << RED_FG << BRIGHT << "Error: " << RESET << message;

	errors.insert(msg.str());
}

/// @brief A helper function for clearing error messages
/// @param errors The reference to the errors set
/// @returns void
void clear_errors(std::set<std::string> &errors)
{
	errors.clear();
};

/// @brief Starts the game and goes on until either checkmate or draw occurs
void Controller::play()
{
	std::shared_ptr<Chess::Player> current_player = white;
	std::shared_ptr<Chess::Player> other_player = black;
	bool check = false;
	bool checkmate = false;
	bool draw = false;
	bool invalid_move = false;
	std::set<std::string> errors;

	Chess::utilities::MoveResult result;

	while (!checkmate && !draw)
	{
		display(current_player.get()->get_name(), checkmate, draw, check);
		std::cout << ": ";
		for (std::string e : errors)
		{
			std::cout << "\n"
						 << e << "\033[A\r: ";
		}

		// Ask for input again if move was invalid
		Chess::Movement mvmt = get_move();
		if (!mvmt.start.is_valid() && !mvmt.end.is_valid())
		{
			clear_errors(errors);
			continue;
		}

		result = board.get()->move(*current_player.get(), *other_player.get(), mvmt);
		invalid_move = result == Chess::utilities::MoveResult::invalid;

		switch (result)
		{
		case Chess::utilities::MoveResult::invalid:
			set_error(errors, "Invalid move. Try again...");
			break;
		case Chess::utilities::MoveResult::ok:
			clear_errors(errors);
			// Swap players
			current_player = current_player == white ? black : white;
			other_player = other_player == white ? black : white;
			// Add movement to history
			history.push_back(mvmt);
			break;

		default:
			// Swap players
			current_player = current_player == white ? black : white;
			other_player = other_player == white ? black : white;
			// Add movement to history
			history.push_back(mvmt);
			break;
		}

		check = result == Chess::utilities::MoveResult::check;
		checkmate = board.get()->is_checkmate(*current_player.get(), *other_player.get());
		draw = board.get()->is_draw(*current_player.get(), *other_player.get());
	}
	display(current_player.get()->get_name(), checkmate, draw, check);

	std::cout << "Game Over...\n\n";
	export_game(); // Right now it only prints the history to the terminal
}

void Controller::display(const std::string &whose_turn, bool is_checkmate, bool is_draw, bool is_check)
{
	const char *checkmate = is_checkmate ? "true" : "false";
	const char *draw = is_draw ? "true" : "false";

	printf("%sNow playing:%s %s	", BRIGHT, RESET, whose_turn.c_str());
	printf("%sCheckmate:%s %s	", BRIGHT, RESET, checkmate);
	printf("%sDraw:%s %s\n\n", BRIGHT, RESET, draw);

	std::cout << *board.get() << "\n\n";
}

/// @brief Exports the game's movements history
/// @details Opens a file stream and saves the movements history to that file
void Controller::export_game()
{
	std::ofstream history_file;

	// printf("%s%sHistory:%s\n", BLUE_FG, BRIGHT, RESET);

	history_file.open("history.txt");

	for (Chess::Movement m : history)
	{
		// std::cout << m << '\n';
		history_file << m << '\n';
	}

	history_file.close();
}