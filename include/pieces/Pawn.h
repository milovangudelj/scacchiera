#ifndef pawn_h
#define pawn_h

#include "Piece.h"
#include "Board.h"

namespace Chess
{
    class PawnPiece : public Piece
    {
        using Piece::Piece;
        public:
        PawnPiece(Coordinate coordinate,Chess::utilities::Color color, Chess::utilities::PieceType type);
        std::list<Movement> get_pseudo_valid_movements(Board& board);
    };
}

#endif