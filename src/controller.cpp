/**
 * @file controller.cpp
 * @author Milovan Gudelj - Mat. 1192574
 * @brief Implementazione della classe Controller che gestisce la partita.
 * @date 2022-22-01
*/

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

using namespace Chess;
using namespace Chess::utilities;

/// Makes subsequent terminal text bright/bold
const char *BRIGHT = "\033[1m";
/// Resets terminal colors to their default values
const char *RESET = "\033[0m";
/// Makes subsequent terminal text red
static const char *RED_FG = "\033[31m";
/// Makes subsequent terminal text blue
static const char *BLUE_FG = "\033[34m";

// Constructors

Controller::Controller(std::string _mode, std::string _fen) : fen(_fen), white(nullptr), black(nullptr), board(nullptr)
{
	init(_mode);
}

Controller::Controller(std::list<std::pair<Movement, char>> _log_list, std::string _fen) : is_replay(true), log_list(_log_list), white(nullptr), black(nullptr), board(nullptr), fen(_fen)
{
	init_replay();
}

// Destructor

Controller::~Controller()
{
	delete white;
	delete black;
	delete board;
}

// Constructor helper methods

void Controller::init(const std::string &_type)
{
	if (_type.compare("pc") == 0)
	{
		srand(time(NULL));
		bool white_is_human = rand() % 2 == 1;

		white = new Player(Color::white, PlayerType{white_is_human ? PlayerType::human : PlayerType::computer});
		black = new Player(Color::black, PlayerType{white_is_human ? PlayerType::computer : PlayerType::human});
	}
	else if (_type.compare("pp") == 0)
	{
		white = new Player(Color::white, PlayerType{PlayerType::human});
		black = new Player(Color::black, PlayerType{PlayerType::human});
	}
	else if (_type.compare("cc") == 0)
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

void Controller::init_replay()
{
	white = new Player(Color::white, PlayerType::computer);
	black = new Player(Color::black, PlayerType::computer);

	board = new Board(fen, white, black);
}

// Utility

void Controller::set_error(const std::string &_message)
{
	std::stringstream msg;
	msg << RED_FG << BRIGHT << "Error: " << RESET << _message << "\033[0K";

	errors.insert(msg.str());
}

void Controller::set_tip(const std::string &_message)
{
	std::stringstream msg;
	msg << BLUE_FG << BRIGHT << "Tip: " << RESET << _message;

	tips.insert(msg.str());
}

std::string capitalise(std::string s)
{
	std::stringstream out("");

	for (size_t i = 0; i < s.length(); i++)
	{
		out << (i == 0 ? char(std::toupper(s[i])) : char(s[i]));
	}

	return out.str();
}

std::string capitalise(Color c)
{
	std::stringstream ss("");
	ss << c;
	return capitalise(ss.str());
}

std::string uppercase(std::string s)
{
	std::stringstream out("");

	for (size_t i = 0; i < s.length(); i++)
	{
		out << char(std::toupper(s[i]));
	}

	return out.str();
}

// Promotion

/**
 * @brief Returns a string with the list of possible promotion candidate pieces.
 * 
 * @param _current_player The currently moving player.
 * @return std::string 
 */
std::string can_promote_to(Player *_current_player)
{
	std::string possible_promotions = "dact";
	std::string out;

	if (_current_player->get_color() == Color::black)
	{
		possible_promotions = uppercase(possible_promotions);
	}

	for (size_t i = 0; i < possible_promotions.size(); i++)
	{
		out += BRIGHT + std::string(1, possible_promotions.at(i)) + RESET + (i == possible_promotions.size() - 1 ? "" : ", ");
	}

	return out;
}

char Controller::promote(Player *_player, char _promote_to)
{
	std::string possible_symbols = "dcat";
	char symbol;

	// Random numbers
	std::random_device dev;	 // Random device
	std::mt19937 rng(dev()); // Random number generator

	bool selected = false;
	int pos;

	while (!selected)
	{
		// If the _promote_to param is given it skips the loop and uses it as the symbol
		if (_promote_to != ' ')
		{
			symbol = _promote_to;
			selected = true;
			continue;
		}

		// If the player is human it prints tip and error messages
		if (_player->get_type() == PlayerType::human)
		{
			std::set<std::string> messages;
			for (std::string e : errors)
			{
				messages.insert(e);
			}
			for (std::string t : tips)
			{
				messages.insert(t);
			}

			int up = 1; // How many lines to move the cursor up by
			for (std::string m : messages)
			{
				up++;
				std::cout << "\n"
							 << m;
			}

			// Cursor control
			std::cout << std::string("\033[" + std::to_string(up) + "A\r: \033[0K");
		}

		// If the player is the computer, the promoted piece gets chosen randomly
		if (_player->get_type() == PlayerType::computer)
		{
			std::uniform_int_distribution<std::mt19937::result_type> piece_dist(0, possible_symbols.size() - 1);
			int random_piece_index = piece_dist(rng);
			symbol = possible_symbols.at(random_piece_index);
		}
		else
		{
			std::cin >> symbol;
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
	if (_promote_to == ' ')
	{
		symbol = english_symbols.at(pos);
	}

	board->promote(*_player, symbol);

	clear_tips();
	clear_errors();

	// Cursor control
	std::cout << (_promote_to != ' ' ? "" : "\033[14A\033[J");
	return symbol;
}

// Match management and flow

Movement Controller::get_move(Player *_current_player)
{
	// If the player is the computer, it choses a movement randomly among the available ones
	if (_current_player->get_type() == PlayerType::computer)
	{
		Movement mvmt = {{9, 9}, {9, 9}};
		std::list<Piece *> available_pieces = _current_player->get_available_pieces();

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

		// While the player has pieces with valid movements
		while (checked_pieces.size() < _current_player->get_available_pieces().size() && found == false)
		{
			// Select a random piece index
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

			// Select a random movement index
			std::uniform_int_distribution<std::mt19937::result_type> mvmt_dist(0, available_movements.size() - 1);
			int random_mvmt_index = mvmt_dist(rng);
			std::list<Movement>::iterator mvmt_it = available_movements.begin();
			std::advance(mvmt_it, random_mvmt_index);

			found = true;
			mvmt = *mvmt_it;
		}

		if (found == false && checked_pieces.size() == _current_player->get_available_pieces().size())
		{
			std::cout << "Player has no more available movements.\n\n";
			exit(0);
		}

		// Cursor control
		std::cout << "\033[13A\033[J";

		return mvmt;
	}

	// Otherwise it asks the player to input a movement in the XX XX format

	std::string from = "";
	std::string to = "";

	std::cin >> from >> to;
	std::transform(from.begin(), from.end(), from.begin(), ::toupper);
	std::transform(to.begin(), to.end(), to.begin(), ::toupper);

	// XX XX is reserved to printing the current board configuration above the interactive one.
	// It serves as a temporary terminal history
	if (from.compare("XX") == 0 && to.compare("XX") == 0)
	{
		return {{9, 9}, {9, 9}};
	}

	// Cursor control
	std::cout << "\033[14A\033[J";

	// Input validation

	std::string input_pattern = "^([A-H][1-8]|XX)$";
	std::basic_regex<char> input_regex = std::regex(input_pattern, std::regex::ECMAScript);
	if (!std::regex_match(from, input_regex) || !std::regex_match(to, input_regex))
	{
		return {{10, 10}, {10, 10}};
	}

	// Movement composition and return

	Coordinate start = {(unsigned int)(8 - (from.at(1) - '0')), (unsigned int)(from.at(0) - 'A')};
	Coordinate end = {(unsigned int)(8 - (to.at(1) - '0')), (unsigned int)(to.at(0) - 'A')};

	return {start, end}; // {{yf, xf},{yt, xt}} where f = from, t = to
}

void Controller::play()
{
	Player *current_player = white;
	Player *other_player = black;
	bool check = false;
	bool checkmate = false;
	bool draw = false;
	bool invalid_move = false;

	MoveResult result;

	while (!checkmate && !draw)
	{
		display(current_player, checkmate, draw, check);

		// Input field printing for human
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
		Movement mvmt = get_move(current_player);

		if (!mvmt.start.is_valid() || !mvmt.end.is_valid())
		{
			if (mvmt.start == Coordinate{9, 9} && mvmt.end == Coordinate{9, 9})
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

		invalid_move = result == MoveResult::invalid;
		check = result == MoveResult::check;
		checkmate = board->is_checkmate(*other_player, *current_player);
		draw = board->is_draw(*other_player, *current_player);

		switch (result)
		{
		case MoveResult::invalid:
			set_error("Invalid move. Try again...");

			break;
		case MoveResult::check:
			set_error("King is under check. Try again...");

			break;
		case MoveResult::ok:
			clear_errors();
			// Swap players
			current_player = current_player == white ? black : white;
			other_player = other_player == white ? black : white;
			// Add movement to history
			history.push_back(std::make_pair(mvmt, ' '));

			if (current_player->get_type() == PlayerType::computer)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

			break;
		case MoveResult::promotion:
			clear_errors();

			display(current_player, checkmate, draw, check);
			std::cout << '\n';

			set_tip("You can promote the pawn in " + mvmt.end + ".\033[0K\n     These are the available pieces: " + can_promote_to(current_player) + "\033[0K\033[A\r");
			char promote_to = promote(current_player);

			checkmate = board->is_checkmate(*other_player, *current_player);
			draw = board->is_draw(*other_player, *current_player);

			// Swap players
			current_player = current_player == white ? black : white;
			other_player = other_player == white ? black : white;

			// Add movement to history
			history.push_back(std::make_pair(mvmt, promote_to));

			break;
		}
	}
	display(current_player, checkmate, draw, check);
	std::cout << "Game Over...\n\n";

	std::cout << "Match result: " << (checkmate ? (BRIGHT + std::string("checkmate") + RESET + "! " + capitalise(other_player->get_color()) + " wins.") : (BRIGHT + std::string("draw") + RESET + "."));

	export_game();
}

std::list<std::string> Controller::replay(char _out)
{
	if (_out != 't' && _out != 'f')
	{
		std::cout << "Output flag must be either 't' (terminal) or 'f' (file).\n\n";
		exit(0);
	}

	std::list<std::string> to_print; // List of strings to be printed to terminal/file

	bool to_terminal = (_out == 't');

	Player *current_player = white;
	Player *other_player = black;
	MoveResult result;
	bool check = false;
	bool checkmate = false;
	bool draw = false;
	bool invalid_move = false;

	if (!is_replay)
	{
		std::cout << "Cannot replay if not provided a list of movements.\n\n";
		exit(0);
	}

	// Create string stream and print initial board layout
	std::stringstream ss;

	if (to_terminal)
	{
		to_print.push_back(display(current_player, checkmate, draw, false, false));
	}
	else
	{
		ss << *board;
		to_print.push_back(ss.str());
	}

	// Loop through the movements, perform and add them to 'to_print'
	std::list<std::pair<Movement, char>>::iterator log_it = log_list.begin();
	for (size_t i = 0; i < log_list.size(); i++)
	{
		// Reset the string stream
		ss.str(std::string());

		// Make the movement
		result = board->move(*current_player, *other_player, (*log_it).first);

		if ((*log_it).second != ' ' && result == MoveResult::promotion)
		{
			promote(current_player, (*log_it).second);
		}

		if (result == MoveResult::invalid)
		{
			std::cout << "There's an invalid movement at line " << BRIGHT << (i + 1) << RESET << " in the history input file. Please fix the mistake and try again.\n\n";
			exit(0);
		}

		// Flags
		check = result == MoveResult::check;
		checkmate = board->is_checkmate(*other_player, *current_player); //checks if enemy is losing
		draw = board->is_draw(*other_player, *current_player);

		// Print the board to the string stream and add it to the list of strings to print
		if (to_terminal)
		{
			ss << display(current_player, checkmate, draw, false, false);
			ss << (i == log_list.size() - 1 ? ("Match result: " + (checkmate ? (BRIGHT + std::string("checkmate") + RESET + "! " + capitalise(current_player->get_color()) + " wins.\n") : (BRIGHT + std::string("draw") + RESET + ".\n"))) : "");
		}
		else
		{
			ss << *board;
			ss << (i == log_list.size() - 1 ? ("\n\nMatch result: " + (checkmate ? ("checkmate! " + capitalise(current_player->get_color()) + " wins.") : ("draw."))) : "");
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

std::string Controller::display(Player *_current_player, bool _is_checkmate, bool _is_draw, bool _is_check, bool _print)
{
	std::stringstream ss;
	std::string out;

	const char *checkmate = _is_checkmate ? "true" : "false";
	const char *draw = _is_draw ? "true" : "false";
	const char *name = _current_player->get_name().c_str();
	const char *color = _current_player->get_color() == Color::white ? "█" : "░";

	ss << BRIGHT << "Now playing:" << RESET << " " << name << " " << color << "	";
	ss << BRIGHT << "Checkmate:" << RESET << " " << checkmate << " 	";
	ss << BRIGHT << "Draw:" << RESET << " " << draw << "\n\n";

	ss << board->pretty_print() << "\n\n";

	out = ss.str();

	if (_print)
		std::cout << out;
	return out;
}

void Controller::export_game()
{
	std::ofstream history_file;

	history_file.open("history.txt");

	history_file << fen << '\n';

	std::list<std::pair<Movement, char>>::iterator history_it = history.begin();
	for (size_t i = 0; i < history.size(); i++)
	{
		history_file << (*history_it).first << " " << (*history_it).second << (i == history.size() - 1 ? "" : "\n");
		std::advance(history_it, 1);
	}

	history_file.close();
}