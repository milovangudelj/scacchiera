/**
 * @file Bishop.h
 * @author Milovan Gudelj - Mat. 1192574
 * @brief Definizione dell'interfaccia di Bishop.
 * @date 2022-22-01
*/

#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

namespace Chess
{
	class Bishop : public Piece
	{
		using Piece::Piece;

	private:
		/**
		 * @brief Color of the cell on which the bishop is standing.
		 */
		Chess::utilities::Color cell_color;

	public:
		// Constructors

		/**
		 * @brief Construct a new Bishop object.
		 * 
		 * @param _coordinate Starting position of the piece.
		 * @param _color Color of the piece.
		 * @param _type The type of the piece. It should be PieceType::bishop in this case.
		 */
		Bishop(Coordinate _coordinate, Chess::utilities::Color _color, Chess::utilities::PieceType _type);

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
		std::list<Chess::Movement> get_pseudo_valid_movements(const Chess::Board &_board);

		Chess::utilities::Color get_cell_color() const { return cell_color; }

		// Utility

		/**
		 * @brief Updates the cell color according to the piece's position on the board.
		 */
		void update_cell_color();
	};
} // namespace Chess

#endif // BISHOP_H