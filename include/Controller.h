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
		std::list<Movement> log_list;
		bool is_replay = false;
		std::shared_ptr<Chess::Player> white;
		std::shared_ptr<Chess::Player> black;
		std::list<Chess::Movement> history;
		std::unique_ptr<Chess::Board> board;
		Chess::Movement get_move();
		void export_game();
		void init(const std::string &type);
		void init_replay();

	public:
		Controller(std::string _mode, std::string _fen);
		Controller(std::list<Chess::Movement> log_list);

		void play();
		std::list<std::string> replay(char out);
		void display(std::shared_ptr<Chess::Player> current_player, bool is_checkmate, bool is_draw, bool is_check = false);
	};
} // namespace Chess

#endif // CONTROLLER_H