#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>
#include <memory>
#include <vector>
#include <set>
#include <string>

#include "Player.h"
#include "Movement.h"
#include "Board.h"

namespace Chess
{
	class Controller
	{
	private:
		std::string fen;
		std::list<std::pair<Chess::Movement, char>> log_list;
		bool is_replay = false;
		Chess::Player *white;
		Chess::Player *black;
		std::list<std::pair<Chess::Movement, char>> history;
		Chess::Board *board;

		// Utility variables and methods
		std::set<std::string> errors;
		std::set<std::string> tips;
		void set_error(const std::string &message);
		void set_tip(const std::string &tip);
		void clear_errors()
		{
			errors.clear();
		};
		void clear_tips()
		{
			tips.clear();
		};

		Chess::Movement get_move(Player *current_player);
		void init(const std::string &type);
		void export_game();
		void init_replay();
		char promote(Player *player, char promote_to = ' ');

	public:
		Controller(std::string _mode, std::string _fen);
		Controller(std::list<std::pair<Chess::Movement, char>> _log_list, std::string _fen);
		~Controller();

		void play();
		std::list<std::string> replay(char out);
		std::string display(Chess::Player *current_player, bool is_checkmate, bool is_draw, bool is_check = false, bool print = true);
	};
} // namespace Chess

#endif // CONTROLLER_H