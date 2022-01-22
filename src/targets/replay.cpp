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

std::list<std::pair<Chess::Movement, char>> get_movements(std::istream &in);
void method_v(std::ifstream &in_f, std::string &s, char c);
void method_f(std::ifstream &in_f, std::ofstream &out_f, std::string &s, char c);
std::vector<std::string> split_it(std::string target, char delimiter);

int main(int argc, char *argv[])
{
	//semplice verifica dei inputi forniti da riga di comando
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

	//comando v
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

	//comando f
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

/********************dal file log, estraggo le mosse e li inserisco nella lista*****************/
std::list<std::pair<Chess::Movement, char>> get_movements(std::istream &in)
{
	std::string s, fen;
	std::list<std::pair<Chess::Movement, char>> movements;
	std::list<std::string> list_string;
	std::list<std::string>::iterator iter;
	char promoted_to = ' ';
	Chess::Coordinate start_c, end_c;
	Chess::Movement move;
	std::pair<Chess::Movement, char> coppia;
	std::vector<std::string> move_vec;
	//spazio di supporto
	while (std::getline(in, s))
	{
		list_string.push_back(s);
	}
	iter = list_string.begin();
	do
	{
		//parto dalla seconda riga di history
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

/********************stampa scacchiera su terminale*****************/
void method_v(std::ifstream &in_f, std::string &fen, char out)
{
	std::list<std::pair<Chess::Movement, char>> log_list;
	std::list<std::string> print_list;
	log_list = get_movements(in_f);

	Chess::Controller controller{log_list, fen};
	print_list = controller.replay(out);

	//stampa a video il replay con pausa di 1s
	std::cout << print_list.front();
	print_list.pop_front();
	for (std::string config : print_list)
	{
		std::cout << "\033[0K\033[J";
		std::cout << config << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

/********************stampo scacchiera su file*****************/
void method_f(std::ifstream &in_f, std::ofstream &out_f, std::string &s, char c)
{
	std::list<std::pair<Chess::Movement, char>> log_list;
	std::list<std::string> print_list;
	log_list = get_movements(in_f);
	//stampa su file il replay senza pausa
	Chess::Controller controller{log_list, s};
	print_list = controller.replay(c);
	std::list<std::string>::iterator print_it = print_list.begin();
	std::list<std::pair<Chess::Movement, char>>::iterator log_it = log_list.begin();
	for (size_t i = 0; i < print_list.size(); i++)
	{
		out_f << "- Movement: " << (*log_it).first << "\n\n";
		out_f << *print_it << (i == print_list.size() - 1 ? "" : "\n\n");
		std::advance(print_it, 1);
		std::advance(log_it, 1);
	}
}

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