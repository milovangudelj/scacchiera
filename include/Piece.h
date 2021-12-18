#ifndef piece_h
#define piece_h

#include "Utilities.h"
#include "Coordinate.h"
#include "Movement.h"
#include "Board.h"
#include <list>

using Chess::utilities::Color;
using Chess::utilities::PieceType;

namespace Chess {

    class Piece {
        protected:
            Color color;
            Coordinate coordinate;
            PieceType type;
            char symbol;
            bool had_moved;


        public:
            Piece(Color color, Coordinate coordinate, PieceType type) :
                color{color}, coordinate{coordinate}, type{type}, had_moved{false} {}
            virtual std::list<Movement> get_pseudo_valid_movements(Board& board) = 0;
            Color get_color() const;
            Coordinate get_coordinate() const;
            PieceType get_type() const;
    };
}

#endif