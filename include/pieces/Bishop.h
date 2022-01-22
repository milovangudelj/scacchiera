// Milovan Gudelj

#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

namespace Chess
{
	class Bishop : public Piece
	{
		using Piece::Piece;

	private:
		Chess::utilities::Color cell_color;

	public:
		Bishop(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
		std::list<Movement> get_pseudo_valid_movements(Board &board);
		Chess::utilities::Color get_cell_color() const;
		void update_cell_color();
	};
} // namespace Chess

#endif // BISHOP_H