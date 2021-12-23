#ifndef knight_piece_h
#define knight_piece_h

#include "Piece.h"

namespace Chess {

    class Knight_Piece : public Piece {
        using Piece::Piece;
        public:
            Knight_Piece(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
            std::list<Movement> get_pseudo_valid_movements(Board& board);
    };
}

#endif