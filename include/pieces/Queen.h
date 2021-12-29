#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

namespace Chess
{
	class Queen : public Piece
	{
		using Piece::Piece;

	public:
		Queen(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
		std::list<Movement> get_pseudo_valid_movements(Board &board);
	};
} // namespace Chess

#endif // QUEEN_H