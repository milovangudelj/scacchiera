#include <iostream>
#include <cstring>
#include <fstream>
#include <list>

#include "Movement.h"
#include "Board.h"

std::list<Chess::Movement> get_movements(std::istream& in);
std::map<char,unsigned int> board_rank;
std::map<char,unsigned int> board_file;

int main(int argc, char *argv[])
{
	//std::cout << "TODO: Add replay logic with argument parsing" << std::endl;
	if(argc < 3)
	{
		std::cerr<<"Dati insufficienti\n";
	}
	if(argc >= 3 && std::strcmp(argv[1],"v") != 0 && std::strcmp(argv[1],"f") != 0)
	{
		std::cerr<<"Comandi inesistenti\n"<<"Comandi disponibili sono: \t"<<"v oppure f\n";
	}
	if(std::strcmp(argv[1],"v") == 0 && argv[2] == nullptr)
	{
		std::cerr<<"file non valido\n";
	}
	if(std::strcmp(argv[1],"f") == 0)
	{
		if(argv[2] == nullptr || argv[3] == nullptr)
		{
			std::cerr<<"file non valido\n";
		}	
	}
	std::ifstream log_ifile;
	std::ofstream log_ofile;
	std::list<Chess::Movement> log_list;
	char out = (std::strcmp(argv[1],"v") ==0 ? 't' : 'f');
	if(std::strcmp(argv[1],"v") == 0)
	{
		log_ifile.open(argv[2]);
		log_list = get_movements(log_ifile);
		//stampa a video il replay con pausa di 1s
		//...passo a controller la lista di movement e stampa a video out=='t'...
		log_ifile.close();
	}
	if(std::strcmp(argv[1],"f") == 0)
	{
		log_ifile.open(argv[2]);
		log_ofile.open(argv[3]);
		log_list = get_movements(log_ifile);
		//stampa su file il replay senza pausa
		//...passo a controller la lista di movement e stampa su file out=='f'...
		log_ifile.close();
		log_ofile.close();
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
		start_c = {board_rank[start_s.at(1)],board_file[start_s.at(0)]};
		end_c = {board_rank[end_s.at(1)],board_file[end_s.at(0)]};
		movements.push_back({start_c,end_c});
	}
	return movements;
}
std::map<char,unsigned int> board_rank
{
	{'1',7},
	{'2',6},
	{'3',5},
	{'4',4},
	{'5',3},
	{'6',2},
	{'7',1},
	{'8',0}
};
std::map<char,unsigned int> board_file
{
	{'A',0},
	{'B',1},
	{'C',2},
	{'D',3},
	{'E',4},
	{'F',5},
	{'G',6},
	{'H',7}
};