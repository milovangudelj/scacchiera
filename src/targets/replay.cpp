#include <iostream>
#include <cstring>
#include <fstream>
#include <list>

#include "Movement.h"
#include "Board.h"
#include "Controller.h"
#include "Player.h"

std::list<Chess::Movement> get_movements(std::istream& in);

int main(int argc, char *argv[])
{
	//semplice verifica dei inputi forniti da riga di comando
	if(argc < 3)
	{
		std::cerr<<"not enough information given\n";
	}
	if(argc <=4 && std::strcmp(argv[1],"v") != 0 && std::strcmp(argv[1],"f") != 0)
	{
		std::cerr<<"ERROR : command not found\n"<<"Available commands are: \t"<<"v or f\n";
	}
	if(argc = 3 && std::strcmp(argv[1],"v") == 0 && argv[2] == nullptr)
	{
		std::cerr<<"file not found\n";
	}
	if(argc = 4 && std::strcmp(argv[1],"f") == 0)
	{
		if(argv[2] == nullptr || argv[3] == nullptr)
		{
			std::cerr<<"file not found\n";
		}	
	}

	std::ifstream log_ifile;
	std::ofstream log_ofile;
	std::list<Chess::Movement> log_list;
	std::list<std::string> print_list;
	char out = (std::strcmp(argv[1],"v") == 0 ? 't' : 'f');
	std::shared_ptr<Chess::Player> white;
	white = std::make_shared<Chess::Player> (Chess::utilities::Color::white, Chess::utilities::PlayerType::computer);
	std::shared_ptr<Chess::Player> black; 
	black = std::make_shared<Chess::Player> (Chess::utilities::Color::black, Chess::utilities::PlayerType::computer);
	std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
	std::shared_ptr<Chess::Board> board;
	board = std::make_shared<Chess::Board> (fen,white,black);

	//comando v
	if(std::strcmp(argv[1],"v") == 0)
	{
		log_ifile.open(argv[2]);
		if(log_ifile.is_open())
		{
			log_list = get_movements(log_ifile);
			//stampa a video il replay con pausa di 1s
			//...passo a controller la lista di movement e stampa a video out=='t'...
			//Chess::Controller controller {log_list};
			//print_list = controller.replay(out);
			std::cout<<*board.get()<<std::endl; //configurazione base
			for(Chess::Movement movement : log_list)
			{
				board.get()->move(*white.get(),*black.get(),movement);
				std::cout<<*board.get()<<"\n";
			}
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
		if(log_ifile.is_open())
		{
			log_list = get_movements(log_ifile);
			//stampa su file il replay senza pausa
			//...passo a controller la lista di movement e stampa su file out=='f'...
			//Chess::Controller controller {log_list};
			//print_list = controller.replay(out);
			std::cout<<*board.get()<<std::endl; //configurazione base
			for(Chess::Movement movement : log_list)
			{
				board.get()->move(*white.get(),*black.get(),movement);
				log_ofile<<*board.get()<<"\n";
			}
			log_ifile.close();
			log_ofile.close();
		}else
		{
			throw std::invalid_argument("File has not been opened properly");
		}
	}
	return 0;
}

//dal file log, estraggo e inserisco nella lista di movement
std::list<Chess::Movement> get_movements(std::istream& in)
{
	std::string s;
	std::list<Chess::Movement> movements;
	std::string start_s, end_s;
	Chess::Coordinate start_c, end_c;
	int pos_whitespace;
	while(std::getline(in,s))
	{
		pos_whitespace = s.find(' ');
		start_s = s.substr(0,pos_whitespace);
		end_s = s.substr(pos_whitespace+1);
		start_c = {(unsigned int)(start_s.at(1)-'0'),(unsigned int)(start_s.at(0)-'A')};
		end_c = {(unsigned int)(end_s.at(1)-'0'),(unsigned int)(end_s.at(0)-'A')};
		movements.push_back({start_c,end_c});
	}
	return movements;
}