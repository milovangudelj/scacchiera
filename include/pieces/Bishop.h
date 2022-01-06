#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

namespace Chess
{
	class Bishop : public Piece
	{
		using Piece::Piece;

	public:
		Bishop(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
		std::list<Movement> get_pseudo_valid_movements(Board &board);
		Chess::utilities::Color get_color() const;
		void set_color(Chess::utilities::Color _color);
	};
} // namespace Chess

#endif // BISHOP_H