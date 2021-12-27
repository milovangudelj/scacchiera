#ifndef king_piece_h
#define king_piece_h

#include "Piece.h"

namespace Chess {

    class KingPiece : public Piece {
        using Piece::Piece;
        public:
            KingPiece(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
            std::list<Movement> get_pseudo_valid_movements(Board& board);
    };
}

#endif