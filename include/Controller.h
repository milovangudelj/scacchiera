#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>
#include <memory>

#include "Player.h"
#include "Movement.h"
#include "Board.h"

namespace Chess
{
	class Controller
	{
	private:
		std::string mode;
		std::shared_ptr<Chess::Player> white;
		std::shared_ptr<Chess::Player> black;
		std::list<Chess::Movement> history;
		std::unique_ptr<Chess::Board> board;
		Chess::Movement get_move();
		void export_game();
		void init(const std::string &type);

	public:
		Controller(std::string mode = "pc");

		void play();
		void display(const Chess::Player *current_player, bool is_checkmate, bool is_draw, bool is_check = false);
	};
} // namespace Chess

#endif // CONTROLLER_H