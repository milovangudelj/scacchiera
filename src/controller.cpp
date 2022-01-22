#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <set>
#include <random>
#include <thread>
#include <utility>
#include <regex>
#include <utility>
#include <algorithm>

#include "Controller.h"
#include "Board.h"

using Chess::Board;
using Chess::Controller;
using Chess::utilities::Color;
using Chess::utilities::PieceType;
using Chess::utilities::PlayerType;

static const char *BRIGHT = "\033[1m";
static const char *RESET = "\033[0m";
static const char *RED_FG = "\033[31m";
static const char *BLUE_FG = "\033[34m";

Controller::Controller(std::string _mode, std::string _fen) : fen(_fen), white(nullptr), black(nullptr), board(nullptr)
{
	init(_mode);
}

Controller::Controller(std::list<std::pair<Movement, char>> _log_list, std::string _fen) : is_replay(true), log_list(_log_list), white(nullptr), black(nullptr), board(nullptr), fen(_fen)
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
	else if (type.compare("pp") == 0)
	{
		white = new Player(Color::white, PlayerType{PlayerType::human});
		black = new Player(Color::black, PlayerType{PlayerType::human});
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
			std::uniform_int_distribution<std::mt19937::result_type> piece_dist(0, available_pieces.size() - 1);

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

			std::uniform_int_distribution<std::mt19937::result_type> mvmt_dist(0, available_movements.size() - 1);
			int random_mvmt_index = mvmt_dist(rng);
			std::list<Movement>::iterator mvmt_it = available_movements.begin();
			std::advance(mvmt_it, random_mvmt_index);

			found = true;
			mvmt = *mvmt_it;
		}

		if (found == false && checked_pieces.size() == current_player->get_available_pieces().size())
		{
			std::cout << "Player has no more available movements.\n\n";
			exit(0);
		}

		std::cout << "\033[0K\033[J";

		return mvmt;
	}

	std::string from = "";
	std::string to = "";

	std::cin >> from >> to;
	std::transform(from.begin(), from.end(), from.begin(), ::toupper);
	std::transform(to.begin(), to.end(), to.begin(), ::toupper);

	if (from.compare("XX") == 0 && to.compare("XX") == 0)
	{
		return {{9, 9}, {9, 9}};
	}

	std::cout << "\033[0K\033[J";

	std::string input_pattern = "^([A-H][1-8]|XX)$";
	std::basic_regex<char> input_regex = std::regex(input_pattern, std::regex::ECMAScript);
	if (!std::regex_match(from, input_regex) || !std::regex_match(to, input_regex))
	{
		return {{10, 10}, {10, 10}};
	}

	Chess::Coordinate start = {(unsigned int)(8 - (from.at(1) - '0')), (unsigned int)(from.at(0) - 'A')};
	Chess::Coordinate end = {(unsigned int)(8 - (to.at(1) - '0')), (unsigned int)(to.at(0) - 'A')};

	return {start, end}; // {{yf, xf},{yt, xt}} where f = from, t = to
}

/// @brief A helper function for setting error messages
/// @param message The error message you want to set
/// @returns void
void Controller::set_error(const std::string &message)
{
	std::stringstream msg;
	msg << RED_FG << BRIGHT << "Error: " << RESET << message << "\033[0K";

	errors.insert(msg.str());
}

/// @brief A helper function for setting error messages
/// @param tip The tip message you want to set
/// @returns void
void Controller::set_tip(const std::string &tip)
{
	std::stringstream msg;
	msg << BLUE_FG << BRIGHT << "Tip: " << RESET << tip;

	tips.insert(msg.str());
}

char get_piece_symbol()
{
	char symbol;
	std::cin >> symbol;
	return symbol;
}

std::string can_promote_to(Chess::Player *player) {
	std::string possible_promotions = "dact";
	std::string output;
	if(player->get_color() == Color::black) {
		std::for_each(possible_promotions.begin(), possible_promotions.end(), 
			[](char &c) {
				c = std::toupper(c);
			}
		);
	}
	for(size_t i = 0; i < possible_promotions.size(); i++) {
		output += BRIGHT + std::string(1, possible_promotions.at(i)) + RESET + (i == possible_promotions.size() - 1 ? "" : ", ");
	}
	return output;
}

char Controller::promote(Player *player, char promote_to)
{
	std::string possible_symbols = "dcat";
	char symbol;

	// Random numbers
	std::random_device dev;
	std::mt19937 rng(dev()); // Random number generator

	bool selected = false;
	int pos;
	while (!selected)
	{
		if (promote_to != ' ')
		{
			symbol = promote_to;
			selected = true;
			continue;
		}

		if(player->get_type() == PlayerType::human) {
			std::set<std::string> messages;
			for (std::string e : errors)
			{
				messages.insert(e);
			}
			for (std::string t : tips)
			{
				messages.insert(t);
			}

			int up = 1; // How many lines to go up by
			for (std::string m : messages)
			{
				up++;
				std::cout << "\n"
							 << m;
			}
			std::cout << std::string("\033[0K\r: ");
		}

		if (player->get_type() == PlayerType::computer)
		{
			std::uniform_int_distribution<std::mt19937::result_type> piece_dist(0, possible_symbols.size() - 1);
			int random_piece_index = piece_dist(rng);
			symbol = possible_symbols.at(random_piece_index);
		}
		else
		{
			symbol = get_piece_symbol();
			symbol = std::tolower(symbol);
		}

		pos = possible_symbols.find_first_of(symbol);
		if (pos == std::string::npos)
		{
			set_error("Invalid piece. Chose a different one.");
			continue;
		}
		selected = true;
	}
	std::string english_symbols = "qnbr";
	symbol = english_symbols.at(pos);
	board->promote(*player, symbol);
	clear_tips();
	clear_errors();
	std::cout << "\033[0K\033[J";
	return symbol;
}

/// @brief Starts the game and goes on until either checkmate or draw occurs
void Controller::play()
{
	Chess::Player *current_player = white;
	Chess::Player *other_player = black;
	bool check = false;
	bool checkmate = false;
	bool draw = false;
	bool invalid_move = false;

	Chess::utilities::MoveResult result;

	while (!checkmate && !draw)
	{
		display(current_player, checkmate, draw, check);

		if (current_player->get_type() == PlayerType::human)
		{
			std::cout << ": ";
			for (std::string e : errors)
			{
				std::cout << "\n"
							 << e << "\033[A\r: ";
			}
		}

		// Ask for input again if move was invalid
		Chess::Movement mvmt = get_move(current_player);

		if (!mvmt.start.is_valid() || !mvmt.end.is_valid())
		{
			if (mvmt.start == Chess::Coordinate{9, 9} && mvmt.end == Chess::Coordinate{9, 9})
			{
				clear_errors();
				std::cout << '\n';
			}
			else
			{
				set_error("Invalid move. Try again...");
			}

			continue;
		}

		result = board->move(*current_player, *other_player, mvmt);
		invalid_move = result == Chess::utilities::MoveResult::invalid;

		checkmate = board->is_checkmate(*other_player, *current_player);
		draw = board->is_draw(*other_player, *current_player);

		check = result == Chess::utilities::MoveResult::check;

		switch (result)
		{
		case Chess::utilities::MoveResult::invalid:
			set_error("Invalid move. Try again...");
			break;
		case Chess::utilities::MoveResult::check:
			set_error("King is under check. Try again...");
			break;
		case Chess::utilities::MoveResult::ok:
			clear_errors();
			// Swap players
			current_player = current_player == white ? black : white;
			other_player = other_player == white ? black : white;
			// Add movement to history
			history.push_back(std::make_pair(mvmt, ' '));

			if(current_player->get_type() == PlayerType::computer) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
			break;
		case Chess::utilities::MoveResult::promotion:
		{
			clear_errors();

			display(current_player, checkmate, draw, check);
			std::cout << '\n';

			set_tip("You can promote the pawn in " + mvmt.end + ".\033[0K\n     These are the available pieces: " + can_promote_to(current_player) + "\033[0K\033[A\r");

			char promote_to = promote(current_player);

			// Swap players
			current_player = current_player == white ? black : white;
			other_player = other_player == white ? black : white;

			// Add movement to history
			history.push_back(std::make_pair(mvmt, promote_to));
			checkmate = board->is_checkmate(*other_player, *current_player);
			draw = board->is_draw(*other_player, *current_player);

			break;
		}
		default:
			// Swap players
			current_player = current_player == white ? black : white;
			other_player = other_player == white ? black : white;
			// Add movement to history
			history.push_back(std::make_pair(mvmt, ' '));
			break;
		}
	}
	display(current_player, checkmate, draw, check);
	std::cout << "Game Over...\n\n";
	export_game();
}

std::list<std::string> Controller::replay(char out)
{
	if (out != 't' && out != 'f')
		throw std::invalid_argument("Output flag must be either 't' (terminal) or 'f' (file).");

	std::list<std::string> to_print; // List of strings to be printed to terminal/file
	bool to_terminal = (out == 't');

	Chess::Player *current_player = white;
	Chess::Player *other_player = black;
	Chess::utilities::MoveResult result;
	bool check = false;
	bool checkmate = false;
	bool draw = false;
	bool invalid_move = false;

	if (!is_replay)
		throw std::invalid_argument("Cannot replay if not provided a list of movements.");

	if (to_terminal)
		to_print.push_back(display(current_player, checkmate, draw, false, false));

	// Create string stream and print initial board layout
	std::stringstream ss;

	// Loop through the movements and add them to 'to_print'
	std::list<std::pair<Movement, char>>::iterator log_it = log_list.begin();
	for (size_t i = 0; i < log_list.size(); i++)
	{
		// Reset the string stream
		ss.str(std::string());

		// Make the movement
		result = board->move(*current_player, *other_player, (*log_it).first);

		if ((*log_it).second != ' ' && result == Chess::utilities::MoveResult::promotion)
		{
			promote(current_player, (*log_it).second);
		}

		if (result == Chess::utilities::MoveResult::invalid)
		{
			std::cout << "There's an invalid movement at line " << BRIGHT << (i + 1) << RESET << " in the history input file. Please fix the mistake and try again.\n\n";
			exit(0);
		}

		// Flags
		check = result == Chess::utilities::MoveResult::check;
		checkmate = board->is_checkmate(*other_player, *current_player); //checks if enemy is losing
		draw = board->is_draw(*other_player, *current_player);

		// Print the board to the string stream and add it to the list of strings to print
		if (to_terminal)
		{
			ss << display(current_player, checkmate, draw, false, false);
		}
		else
		{
			ss << *board;
			std::string result = "\n\nMatch result: " + (checkmate ? (std::string("checkmate for ") + current_player->get_color()) : "draw");
			ss << (i == log_list.size() - 1 ? result : "");
		}
		to_print.push_back(ss.str());

		// Swap players
		Player *temp = current_player;
		current_player = other_player;
		other_player = temp;

		// Advance iterator
		std::advance(log_it, 1);
	}

	return to_print;
}

std::string Controller::display(Player *current_player, bool is_checkmate, bool is_draw, bool is_check, bool print)
{
	std::stringstream ss;
	std::string out;

	const char *checkmate = is_checkmate ? "true" : "false";
	const char *draw = is_draw ? "true" : "false";
	const char *name = current_player->get_name().c_str();
	const char *color = current_player->get_color() == Chess::utilities::Color::white ? "█" : "░";

	ss << BRIGHT << "Now playing:" << RESET << " " << name << " " << color << "	";
	ss << BRIGHT << "Checkmate:" << RESET << " " << checkmate << " 	";
	ss << BRIGHT << "Draw:" << RESET << " " << draw << "\n\n";

	ss << board->pretty_print() << "\n\n";

	out = ss.str();

	if (print)
		std::cout << out;
	return out;
}

/// @brief Exports the game's movements history
/// @details Opens a file stream and saves the movements history to that file
void Controller::export_game()
{
	std::ofstream history_file;

	history_file.open("history.txt");

	history_file << fen << '\n';

	std::list<std::pair<Chess::Movement, char>>::iterator history_it = history.begin();
	for (size_t i = 0; i < history.size(); i++)
	{
		history_file << (*history_it).first << " " << (*history_it).second << (i == history.size() - 1 ? "" : "\n");
		std::advance(history_it, 1);
	}

	history_file.close();
}