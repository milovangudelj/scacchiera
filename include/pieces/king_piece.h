#ifndef king_piece_h
#define king_piece_h

#include "piece.h"

namespace Chess {

    class KingPiece : public Piece {
        using Piece::Piece;
        public:
            KingPiece(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
            std::list<Movement> get_pseudo_valid_movements(Board& board);
    };

    Coordinate get_testing_coordinate(Coordinate starting_coordinate, Direction direction) {
        return starting_coordinate + DirectionOffset.at(direction);
    }

}

#endif