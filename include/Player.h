/**
 * @file Player.h
 * @author Milovan Gudelj - Mat. 1192574
 * @brief Definizione dell'interfaccia di Player.
 * @date 2022-22-01
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>
#include <ostream>

#include "Utilities.h"
#include "Coordinate.h"
#include "Piece.h"

namespace Chess
{
	class Player
	{
	private:
		std::string name;
		Chess::utilities::PlayerType type;
		Chess::utilities::Color color;
		std::list<std::unique_ptr<Chess::Piece>> available_pieces;
		std::list<std::unique_ptr<Chess::Piece>> lost_pieces;
		std::list<Chess::Piece *> available_pieces_copy;
		std::list<Chess::Piece *> lost_pieces_copy;
		int stale_since;

	public:
		// Constructors

		/**
		 * @brief Constructs a new Player object.
		 * 
		 * @param _color Color of the player.
		 * @param _type Type of the player. Either PlayerType::human or PlayerType::computer.
		 * @param _name The name of the player. By default "Human" and "Hal" respectively for a human and a computer player.
		 */
		Player(const Chess::utilities::Color &_color, const Chess::utilities::PlayerType &_type, const std::string &_name);
		Player(const Chess::utilities::Color &_color, const Chess::utilities::PlayerType &_type) : Player(_color, _type, ""){};
		Player(const Chess::utilities::Color &_color) : Player(_color, Chess::utilities::PlayerType::human){};
		Player() : Player(Chess::utilities::Color::white){};

		// Simple inline getters

		const std::list<Chess::Piece *> &get_available_pieces() const { return available_pieces_copy; };
		const std::list<Chess::Piece *> &get_lost_pieces() const { return lost_pieces_copy; };

		Chess::utilities::PlayerType get_type() const { return type; };
		Chess::utilities::Color get_color() const { return color; };
		std::string get_name() const { return name; }

		int get_stale_since() const { return stale_since; };

		// Player management member functions

		void move_to_lost_pieces(Chess::Piece *_piece);
		void move_to_available_pieces(Chess::Piece *_piece);

		/**
		 * @brief Adds a new piece to the list of available ones
		 * 
		 * @param _coordinate Starting position of the piece.
		 * @param _color Color of the piece.
		 * @param _type The type of the piece.
		 * @return A pointer to the newly created piece.
		 */
		Chess::Piece *add_to_available_pieces(Chess::Coordinate _coordinate, Chess::utilities::Color _color, Chess::utilities::PieceType _type);

		void increment_stale_since() { stale_since++; };
		void reset_stale_since() { stale_since = 0; };

		// Overloaded operators

		friend std::ostream &operator<<(std::ostream &_os, const Player &_player);
	};

} // namespace Chess

#endif // PLAYER_H