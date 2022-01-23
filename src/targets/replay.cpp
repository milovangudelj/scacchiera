/**
 * @file replay.cpp
 * @author Tommaso Jin
 * @brief  Reading from log and replaying the game
 */

#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <list>
#include <thread>
#include <chrono>
#include <utility>

#include "Movement.h"
#include "Controller.h"

/// @brief to get the list of movement with possible promotion's char
/// @param in reference to istream
/// @returns list<pair<Movement,char>>
std::list<std::pair<Chess::Movement, char>> get_movements(std::istream &in);

/// @brief to print baord on terminal
/// @param in_f reference to ifstream
/// @param fen reference to string, describes the inizial position of all pieces
/// @param out to state what output should be(terminal or file)
/// @returns void
void method_v(std::ifstream &in_f, std::string &fen, char out);

/// @brief to print baord on file
/// @param in_f reference to ifstream
/// @param out_f reference to ofstream
/// @param fen reference to string, describes the inizial position of all pieces
/// @param out to state what output should be(terminal or file)
/// @returns void
void method_f(std::ifstream &in_f, std::ofstream &out_f, std::string &fen, char out);

/// @brief tokenize string into vector of string
/// @param target string to tokenize
/// @param delimiter char that separates each token
/// @returns vector<string>
std::vector<std::string> split_it(std::string target, char delimiter);

int main(int argc, char *argv[])
{
	// simple controll of command line input
	if (argc < 3)
	{
		std::cerr << "not enough information given\n";
	}
	if (std::strcmp(argv[1], "v") != 0 && std::strcmp(argv[1], "f") != 0)
	{
		std::cerr << "ERROR : command not found\n"
				  << "Available commands are: \t"
				  << "v or f\n";
	}
	if (std::strcmp(argv[1], "v") == 0 && argc != 3)
	{
		std::cerr << "file not found\n";
	}
	if (std::strcmp(argv[1], "f") == 0 && argc != 4)
	{
		std::cerr << "file not found\n";
	}
	if (argc > 4)
	{
		if (std::strcmp(argv[1], "v") == 0)
		{
			std::cerr << "should provide only one file\n";
		}
		if (std::strcmp(argv[1], "f") == 0)
		{
			std::cerr << "should provide two files\n";
		}
	}

	std::ifstream log_ifile;
	std::ofstream log_ofile;
	std::list<std::pair<Chess::Movement, char>> log_list;
	std::list<std::string> print_list;
	std::string fen;
	char out = (std::strcmp(argv[1], "v") == 0 ? 't' : 'f');

	// command v
	if (std::strcmp(argv[1], "v") == 0)
	{
		log_ifile.open(argv[2]);

		if (log_ifile.is_open())
		{
			std::getline(log_ifile, fen);
			method_v(log_ifile, fen, out);

			log_ifile.close();
		}
		else
		{
			throw std::invalid_argument("File has not been opened properly");
		}
	}

	// command f
	if (std::strcmp(argv[1], "f") == 0)
	{
		log_ifile.open(argv[2]);
		log_ofile.open(argv[3]);

		if (log_ifile.is_open() && log_ofile.is_open())
		{
			std::getline(log_ifile, fen);
			method_f(log_ifile, log_ofile, fen, out);

			log_ifile.close();
			log_ofile.close();
		}
		else
		{
			throw std::invalid_argument("File has not been opened properly");
		}
	}

	return 0;
}

/********************from log file, to extract and to insert into list of pair*****************/
std::list<std::pair<Chess::Movement, char>> get_movements(std::istream &in)
{
	std::string s;
	std::list<std::pair<Chess::Movement, char>> movements;
	std::list<std::string> list_string;
	std::list<std::string>::iterator iter;
	char promoted_to = ' ';
	Chess::Coordinate start_c, end_c;
	Chess::Movement move;
	std::pair<Chess::Movement, char> coppia;
	std::vector<std::string> move_vec;

	// buffer
	while (std::getline(in, s))
	{
		list_string.push_back(s);
	}

	iter = list_string.begin();

	do
	{
		// starts from file's second line
		move_vec = split_it(*iter, ' ');

		start_c = {static_cast<unsigned int>(8 - (move_vec[0].at(1) - '0')), static_cast<unsigned int>(move_vec[0].at(0) - 'A')};
		end_c = {static_cast<unsigned int>(8 - (move_vec[1].at(1) - '0')), static_cast<unsigned int>(move_vec[1].at(0) - 'A')};

		if (move_vec.size() == 3)
		{
			promoted_to = move_vec[2].at(0);
		}

		move = {start_c, end_c};
		coppia = std::make_pair(move, promoted_to);
		movements.push_back(coppia);

		advance(iter, 1);

	} while (iter != list_string.end());

	return movements;
}

/********************to print board on terminal*****************/
void method_v(std::ifstream &in_f, std::string &fen, char out)
{
	std::list<std::pair<Chess::Movement, char>> log_list;
	std::list<std::string> print_list;

	log_list = get_movements(in_f);

	Chess::Controller controller{log_list, fen};
	print_list = controller.replay(out);

	// printing with 1 second delay

	std::list<std::string>::iterator print_it = print_list.begin();

	for (size_t i = 0; i < print_list.size(); i++)
	{
		std::cout << *print_it;

		if (i != print_list.size() - 1)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "\033[13A\033[J";
		}

		std::advance(print_it, 1);
	}
}

/********************to print on file*****************/
void method_f(std::ifstream &in_f, std::ofstream &out_f, std::string &fen, char out)
{
	std::list<std::pair<Chess::Movement, char>> log_list;
	std::list<std::string> print_list;

	log_list = get_movements(in_f);

	// printing without delay
	Chess::Controller controller{log_list, fen};
	print_list = controller.replay(out);

	std::list<std::string>::iterator print_it = print_list.begin();
	std::list<std::pair<Chess::Movement, char>>::iterator log_it = log_list.begin();

	out_f << "Start:\n\n"
		  << *print_it << "\n\n";
	std::advance(print_it, 1);

	for (size_t i = 0; i < log_list.size(); i++)
	{
		out_f << "- Move: " << (*log_it).first << "\n\n";
		out_f << *print_it << (i == log_list.size() - 1 ? "" : "\n\n");

		std::advance(print_it, 1);
		std::advance(log_it, 1);
	}
}

// utility for get_movements
std::vector<std::string> split_it(std::string target, char delimiter)
{
	std::stringstream ss(target);
	std::string piece;
	std::vector<std::string> pieces;

	while (std::getline(ss, piece, delimiter))
	{
		if (piece.compare("") == 0)
			continue;

		pieces.push_back(piece);
	}

	return pieces;
}