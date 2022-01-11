#include <memory>
#include <iostream>
#include <sstream>

#include "Controller.h"
#include "Board.h"

using Chess::Board;
using Chess::Controller;
using Chess::utilities::Color;
using Chess::utilities::PlayerType;

#define BRIGHT "\033[1m"
#define RESET "\033[0m"

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

	board = std::make_unique<Board>("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", *white.get(), *black.get());
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

void Controller::play()
{
	bool check = false;
	bool checkmate = false;
	bool draw = false;

	Chess::utilities::MoveResult result;

	while (!checkmate && !draw)
	{
		display(checkmate, draw, check);
		std::cout << ": ";

		Chess::Movement mvmt = get_move();
		if (!mvmt.start.is_valid() && !mvmt.end.is_valid())
			continue;

		result = board.get()->move(*white.get(), *black.get(), mvmt);
		if (result == Chess::utilities::MoveResult::invalid)
		{
			std::cout << "Invalid move...";
		}

		check = result == Chess::utilities::MoveResult::check;
		checkmate = board.get()->is_checkmate(*white.get(), *black.get());
		draw = board.get()->is_draw(*white.get(), *black.get());
	}
	display(checkmate, draw, check);

	std::cout << "Game Over...\n\n";
}

void Controller::display(bool is_checkmate, bool is_draw, bool is_check)
{
	std::string checkmate = is_checkmate ? "true" : "false";
	std::string draw = is_draw ? "true" : "false";

	std::cout << BRIGHT // Bright
				 << "Checkmate:"
				 << RESET << " " << checkmate << "   "
				 << BRIGHT
				 << "Draw:"
				 << RESET << " " << draw << "\n\n";

	std::cout << *board.get() << "\n\n";
}