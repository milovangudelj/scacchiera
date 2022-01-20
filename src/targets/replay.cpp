#include <iostream>
#include <cstring>
#include <fstream>
#include <list>
#include <thread>
#include <chrono>

#include "Movement.h"
#include "Controller.h"

std::list<Chess::Movement> get_movements(std::istream& in);
void method_v(std::ifstream& in_f,char c);
void method_f(std::ifstream& in_f,std::ofstream& out_f,char c);

int main(int argc, char *argv[])
{
	//semplice verifica dei inputi forniti da riga di comando
	if(argc < 3)
	{
		std::cerr<<"not enough information given\n";
	}
	if( std::strcmp(argv[1],"v") != 0 && std::strcmp(argv[1],"f") != 0)
	{
		std::cerr<<"ERROR : command not found\n"<<"Available commands are: \t"<<"v or f\n";
	}
	if(std::strcmp(argv[1],"v") == 0 && argc != 3)
	{
		std::cerr<<"file not found\n";
	}
	if(std::strcmp(argv[1],"f") == 0 && argc != 4)
	{
		std::cerr<<"file not found\n";
	}
	if(argc > 4)
	{
		if(std::strcmp(argv[1],"v") == 0)
		{
			std::cerr<<"should provide only one file\n";
		}
		if(std::strcmp(argv[1],"f") == 0)
		{
			std::cerr<<"should provide two files\n";
		}
	}

	std::ifstream log_ifile;
	std::ofstream log_ofile;
	std::list<Chess::Movement> log_list;
	std::list<std::string> print_list;
	char out = (std::strcmp(argv[1],"v") == 0 ? 't' : 'f');

	//comando v
	if(std::strcmp(argv[1],"v") == 0)
	{
		log_ifile.open(argv[2]);
		if(log_ifile.is_open())
		{
			method_v(log_ifile,out);
			log_ifile.close();
		}else
		{
			throw std::invalid_argument("File has not been opened properly");
		}
	}
	
	//comando f
	if(std::strcmp(argv[1],"f") == 0)
	{
		log_ifile.open(argv[2]);
		log_ofile.open(argv[3]);
		if(log_ifile.is_open() && log_ofile.is_open())
		{
			method_f(log_ifile,log_ofile,out);
			log_ifile.close();
			log_ofile.close();
		}else
		{
			throw std::invalid_argument("File has not been opened properly");
		}
	}
	
	return 0;
}

/********************dal file log, estraggo e inserisco nella lista di movement*****************/
std::list<Chess::Movement> get_movements(std::istream& in)
{
	std::string s;
	std::list<Chess::Movement> movements;
	std::string start_s, end_s;
	Chess::Coordinate start_c, end_c;
	int pos_whitespace;
	//while(std::getline(in,s))
	while(in>>start_s>>end_s)
	{
		//pos_whitespace = s.find(' ');
		//start_s = s.substr(0,pos_whitespace);
		//end_s = s.substr(pos_whitespace+1);
		start_c = {static_cast<unsigned int>(8-(start_s.at(1)-'0')),static_cast<unsigned int>(start_s.at(0)-'A')};
		end_c = {static_cast<unsigned int>(8-(end_s.at(1)-'0')),static_cast<unsigned int>(end_s.at(0)-'A')};
		movements.push_back({start_c,end_c});
	}
	return movements;
}

/********************stampa scacchiera su terminale*****************/
void method_v(std::ifstream& in_f, char c )
{
	std::list<Chess::Movement> log_list;
	std::list<std::string> print_list;
	log_list = get_movements(in_f);
	//stampa a video il replay con pausa di 1s
	Chess::Controller controller {log_list};
	print_list = controller.replay(c);
	std::cout<<print_list.front();
	print_list.pop_front();
	for(std::string config : print_list)
	{
		std::cout << "\033[13A\033[J";
		std::cout << config << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

/********************stampo scacchiera su file*****************/
void method_f(std::ifstream& in_f,std::ofstream& out_f,char c)
{
	std::list<Chess::Movement> log_list;
	std::list<std::string> print_list;
	log_list = get_movements(in_f);
	//stampa su file il replay senza pausa
	Chess::Controller controller {log_list};
	print_list = controller.replay(c);
	for(std::string config : print_list)
	{
		out_f<<config<<"\n";
	}
}