#ifndef king_piece_h
#define king_piece_h

#include "Piece.h"
#include "Utilities.h"

namespace Chess {

    class KingPiece : public Piece {
        using Piece::Piece;
        public:
            KingPiece(Color color, Coordinate coordinate, PieceType type);
            std::list<Movement> get_pseudo_valid_movements(Board& board);

    };
}

#endif