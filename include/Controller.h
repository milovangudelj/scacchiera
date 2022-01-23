/**
 * @file Controller.h
 * @author Milovan Gudelj - Mat. 1192574
 * @brief Definizione dell'interfaccia di Controller.
 * @date 2022-22-01
*/

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

		// Utility member variables and methods

		std::set<std::string> errors;
		std::set<std::string> tips;

		void set_error(const std::string &_message);
		void set_tip(const std::string &_message);

		void clear_errors() { errors.clear(); }
		void clear_tips() { tips.clear(); }

		// Match management methods

		/**
		 * @brief Gets the next move of the current player from standard input.
		 *        If the player is the computer, the move is 
		 *        chosen randomly among those available.
		 * 
		 * @param _current_player The currently moving player.
		 * @return Chess::Movement 
		 */
		Chess::Movement get_move(Player *_current_player);

		/**
		 * @brief Exports the game's movements history.
		 *        Opens a file stream and saves the movements history to that file.
		 */
		void export_game();

		/**
		 * @brief Promotion flow method.
		 * 
		 * @param _player The player that needs to perform a promotion.
		 * @param _promote_to (optional) - The piece to which the pawn needs to be promoted to.
		 * @return The symbol of the promoted piece.
		 */
		char promote(Player *_player, char _promote_to = ' ');

		// Constructor helper methods

		/**
		 * @brief Initialises the controller by instantiating a board object 
		 *        and chosing randomly who gets to play as white in pc matches.
		 * 
		 * @param _mode The mode of the Controller. One of ['pc', 'cc', 'pp'].
		 */
		void init(const std::string &_mode);

		/**
		 * @brief In a similar fashion to Controller::init(), 
		 *        it initialises the controller with two computer players.
		 */
		void init_replay();

	public:
		// Constructors

		/**
		 * @brief Constructs a new Controller object in the given mode and starting configuration.
		 * 
		 * @param _mode The mode of the Controller. One of ['pc', 'cc', 'pp'].
		 * @param _fen Forsyth–Edwards Notation (FEN) string representing the initial state of the board
		 */
		Controller(std::string _mode, std::string _fen);

		/**
		 * @brief Constructs a new Controller object with the given log list and starting configuration.
		 * 
		 * @param _log_list The list of movements to replay.
		 * @param _fen Forsyth–Edwards Notation (FEN) string representing the initial state of the board
		 */
		Controller(std::list<std::pair<Chess::Movement, char>> _log_list, std::string _fen);

		// Destructor

		~Controller();

		// Match flow

		/**
		 * @brief Starts the game and goes on until either checkmate or draw occurs
		 */
		void play();

		/**
		 * @brief Replays a history of movements in a similar fashion to Controller::play(), and
		 *        it generates a list of strings to be printed either to the terminal or to a file.
		 * 
		 * @param _out Where will the output end up. Either 't' (terminal) or 'f' (file).
		 * @return A list of strings to be printed.
		 */
		std::list<std::string> replay(char _out);

		/**
		 * @brief Based on the _print parameter it either displays the current board 
		 *        configuration to the terminal, or it returns it as a string for future use.
		 * 
		 * @param _current_player The currently moving player.
		 * @param _is_checkmate Whether the match is in a checkmate state or not.
		 * @param _is_draw Whether the match is in a draw state or not.
		 * @param _is_check (optional) - Whether the current player's king is in under check or not.
		 * @param _print (optional) - Whether to print the configuration to the terminal or not.
		 * @return The current board configuration and info about it's state.
		 */
		std::string display(Chess::Player *_current_player, bool _is_checkmate, bool _is_draw, bool _is_check = false, bool _print = true);
	};
} // namespace Chess

#endif // CONTROLLER_H