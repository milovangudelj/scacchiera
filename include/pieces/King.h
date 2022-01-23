/**
 * @file King.h
 * @author Yihui Zhu
 * @brief Header file of King class
 */

#ifndef king_h
#define king_h

#include "Piece.h"
#include "Utilities.h"

namespace Chess {

    class King : public Piece {

        using Piece::Piece; //Base class constructor

        public:

            King(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);

            std::list<Movement> get_pseudo_valid_movements(const Board& board);
    };

}

#endif