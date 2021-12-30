#ifndef king_h
#define king_h

#include "Piece.h"
#include "Utilities.h"

namespace Chess {

    class King : public Piece {
        using Piece::Piece;
        public:
            King(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
            std::list<Movement> get_pseudo_valid_movements(Board& board);
    };

    Coordinate get_testing_coordinate(Coordinate starting_coordinate, Chess::utilities::Direction direction) {
        return starting_coordinate + Chess::utilities::DirectionOffset.at(direction);
    }

}

#endif