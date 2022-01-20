#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>
#include <memory>
#include <vector>

#include "Player.h"
#include "Movement.h"
#include "Board.h"

namespace Chess
{
	class Controller
	{
	private:
		std::string fen;
		std::list<Chess::Movement> log_list;
		bool is_replay = false;
		Chess::Player *white;
		Chess::Player *black;
		std::list<Chess::Movement> history;
		Chess::Board *board;
		Chess::Movement get_move(Player *current_player);
		void export_game();
		void init(const std::string &type);
		void init_replay();
		void promote(Player *player);

	public:
		Controller(std::string _mode, std::string _fen);
		Controller(std::list<Chess::Movement> log_list);
		~Controller();

		void play();
		std::list<std::string> replay(char out);
		std::string display(Chess::Player *current_player, bool is_checkmate, bool is_draw, bool is_check = false, bool print = true);
	};
} // namespace Chess

#endif // CONTROLLER_H