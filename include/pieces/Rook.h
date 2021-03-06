/**
 * @file Rook.h
 * @author Tommaso Jin
 * @brief  Header file of Rook class
 */

#ifndef Rook_h
#define Rook_h

#include "Piece.h"

namespace Chess
{
	class Rook : public Piece
	{
		using Piece::Piece;

	public:
		Rook(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
 		std::list<Movement> get_pseudo_valid_movements(const Board& board);
	};
}

#endif