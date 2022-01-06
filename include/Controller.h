#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>

#include "Player.h"
#include "Movement.h"

namespace Chess
{
	class Controller
	{
	private:
		Chess::Player white;
		Chess::Player black;
		std::list<Chess::Movement> history;
		void display();
		void play();
		void export_game();

	public:
		Controller(/* args */);
		~Controller();
	};
} // namespace Chess

#endif // CONTROLLER_H