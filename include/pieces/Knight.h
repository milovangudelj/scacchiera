/**
 * @file Knight.h
 * @author Yihui Zhu
 * @brief Header file for Knight class
 */

#ifndef knight_h
#define knight_h

#include "Piece.h"

namespace Chess {

    class Knight : public Piece {

        using Piece::Piece; //Base class constructor

        public:

            Knight(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);

            std::list<Movement> get_pseudo_valid_movements(const Board& board);
    };
}

#endif