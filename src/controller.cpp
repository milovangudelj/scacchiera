#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <random>
#include <thread>

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

Controller::Controller(std::string _mode, std::string _fen) : fen(_fen), white(nullptr), black(nullptr), board(nullptr)
{
	init(_mode);
}

Controller::Controller(std::list<Movement> _log_list) : is_replay(true), log_list(_log_list), white(nullptr), black(nullptr), board(nullptr)
{
	init_replay();
}

Controller::~Controller()
{
	delete white;
	delete black;
	delete board;
}

void Controller::init_replay()
{
	white = new Player(Color::white, PlayerType::computer);
	black = new Player(Color::black, PlayerType::computer);
	fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";

	board = new Board(fen, white, black);
}

void Controller::init(const std::string &type)
{
	if (type.compare("pc") == 0)
	{
		srand(time(NULL));
		bool white_is_human = rand() % 2 == 1;

		white = new Player(Color::white, PlayerType{white_is_human ? PlayerType::human : PlayerType::computer});
		black = new Player(Color::black, PlayerType{white_is_human ? PlayerType::computer : PlayerType::human});
	}
	else if (type.compare("cc") == 0)
	{
		white = new Player(Color::white, PlayerType::computer);
		black = new Player(Color::black, PlayerType::computer);
	}
	else
	{
		throw "Invalid game type";
	}

	board = new Board(fen, white, black);
}

Chess::Movement Controller::get_move(Player *current_player)
{
	if (current_player->get_type() == PlayerType::computer)
	{
		Movement mvmt = {{9, 9}, {9, 9}};
		std::list<Chess::Piece *> available_pieces = current_player->get_available_pieces();

		if (available_pieces.size() == 0)
		{
			std::cout << "Player has no more available pieces.\n\n";
			exit(0);
		}

		std::list<Piece *> checked_pieces;
		bool found = false;

		// Random numbers
		std::random_device dev;
		std::mt19937 rng(dev()); // Random number generator

		while (checked_pieces.size() < current_player->get_available_pieces().size() && found == false)
		{
			std::uniform_int_distribution<std::mt19937::result_type> piece_dist(0, available_pieces.size());

			int random_piece_index = piece_dist(rng);
			std::list<Piece *>::iterator piece_it = available_pieces.begin();
			std::advance(piece_it, random_piece_index);

			Piece *piece = *piece_it;
			checked_pieces.push_back(piece);

			std::list<Movement> available_movements = piece->get_pseudo_valid_movements(*board);
			if (available_movements.size() == 0)
			{
				available_pieces.remove(piece);
				continue;
			}

			std::uniform_int_distribution<std::mt19937::result_type> mvmt_dist(0, available_movements.size());
			int random_mvmt_index = mvmt_dist(rng);
			std::list<Movement>::iterator mvmt_it = available_movements.begin();
			std::advance(mvmt_it, random_piece_index);

			std::cout << "\033[13A\033[J";
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			found = true;
			mvmt = *mvmt_it;
		}

		if (found == false && checked_pieces.size() == current_player->get_available_pieces().size())
		{
			std::cout << "Player has no more available movements.\n\n";
			exit(0);
		}

		return mvmt;
	}

	std::string from = "";
	std::string to = "";

	std::cin >> from >> to;
	std::cout << "\033[14A\033[J";

	if (from.compare("XX") == 0 && to.compare("XX") == 0)
	{
		return {{9, 9}, {9, 9}};
	}

	Chess::Coordinate start = {(unsigned int)(8 - (from.at(1) - '0')), (unsigned int)(from.at(0) - 'A')};
	Chess::Coordinate end = {(unsigned int)(8 - (to.at(1) - '0')), (unsigned int)(to.at(0) - 'A')};

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
	Chess::Player *current_player = white;
	Chess::Player *other_player = black;
	bool check = false;
	bool checkmate = false;
	bool draw = false;
	bool invalid_move = false;
	std::set<std::string> errors;

	Chess::utilities::MoveResult result;

	while (!checkmate && !draw)
	{
		display(current_player, checkmate, draw, check);
		std::cout << ": ";
		for (std::string e : errors)
		{
			std::cout << "\n"
						 << e << "\033[A\r: ";
		}

		// Ask for input again if move was invalid
		Chess::Movement mvmt = get_move(current_player);

		if (!mvmt.start.is_valid() && !mvmt.end.is_valid())
		{
			clear_errors(errors);
			continue;
		}

		result = board->move(*current_player, *other_player, mvmt);
		invalid_move = result == Chess::utilities::MoveResult::invalid;

		check = result == Chess::utilities::MoveResult::check;
		checkmate = board->is_checkmate(*other_player, *current_player); //checks if enemy is losing
		draw = board->is_draw(*current_player, *other_player);

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
	}
	display(current_player, checkmate, draw, check);

	std::cout << "Game Over...\n\n";
	export_game(); // Right now it only prints the history to the terminal
}

std::list<std::string> Controller::replay(char out)
{
	if (out != 't' && out != 'f')
		throw std::invalid_argument("Output flag must be either 't' (terminal) or 'f' (file).");

	std::list<std::string> to_print; // List of strings to be printed to terminal/file
	bool to_terminal = (out == 't');

	Chess::Player *current_player = white;
	Chess::Player *other_player = black;
	bool check = false;
	bool checkmate = false;
	bool draw = false;
	bool invalid_move = false;

	if (!is_replay)
		throw std::invalid_argument("Cannot replay if not provided a list of movements.");

	if (to_terminal)
		display(current_player, checkmate, draw);

	// Create string stream and print initial board layout
	std::stringstream ss;
	ss << *board;
	to_print.push_back(ss.str());

	// Loop through the movements and add them to 'to_print'
	for (Movement movement : log_list)
	{
		// Reset the string stream
		ss.str(std::string());

		// Make teh movement
		board->move(*current_player, *other_player, movement);

		// Print the board to the string stream and add it to the list of strings to print
		if (to_terminal)
		{
			ss << board->pretty_print();
		}
		else
		{
			ss << *board;
		}
		to_print.push_back(ss.str());

		// Swap players
		Player *temp = current_player;
		current_player = other_player;
		other_player = temp;
	}

	return to_print;
}

void Controller::display(Player *current_player, bool is_checkmate, bool is_draw, bool is_check)
{
	const char *checkmate = is_checkmate ? "true" : "false";
	const char *draw = is_draw ? "true" : "false";
	const char *name = current_player->get_name().c_str();
	const char *color = current_player->get_color() == Chess::utilities::Color::white ? "█" : "░";

	printf("%sNow playing:%s %s %s	", BRIGHT, RESET, name, color);
	printf("%sCheckmate:%s %s	", BRIGHT, RESET, checkmate);
	printf("%sDraw:%s %s\n\n", BRIGHT, RESET, draw);

	std::cout << board->pretty_print() << "\n\n";
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