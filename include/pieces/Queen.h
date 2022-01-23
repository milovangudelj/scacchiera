/**
 * @file Queen.h
 * @author Milovan Gudelj - Mat. 1192574
 * @brief Definizione dell'interfaccia di Queen.
 * @date 2022-22-01
*/

#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

namespace Chess
{
	class Queen : public Piece
	{
		using Piece::Piece;

	public:
		// Constructors

		/**
		 * @brief Construct a new Queen object.
		 * 
		 * @param _coordinate Starting position of the piece.
		 * @param _color Color of the piece.
		 * @param _type The type of the piece. It should be PieceType::queen in this case.
		 */
		Queen(Chess::Coordinate _coordinate, Chess::utilities::Color _color, Chess::utilities::PieceType _type);

		// Getters

		/**
		 * @brief Generates a list of possible movements regardless of their outcome.
		 *        The method is not aware of the consequences of the movements it deems "pseudo-valid".
		 *        The list will be manipulated further down the line in order to discard those
		 *        that can lead to the king being left exposed.
		 * 
		 * @param _board Reference to the board object from where the piece needs to find the moves.
		 * @return std::list< Chess::Movement >
		 */
		std::list<Chess::Movement> get_pseudo_valid_movements(Chess::Board &_board);
	};
} // namespace  Chess

#endif // QUEEN_H