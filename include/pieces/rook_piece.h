#ifndef Rook_Piece_h
#define Rook_Piece_h

#include"piece.h"

namespace Chess
{
	class RookPiece : public Piece
	{
        using Piece::Piece;
        public:
		RookPiece(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
 		std::list<Movement> get_pseudo_valid_movements(Board& board);
	};
}

#endif