#ifndef piece_h
#define piece_h

#include "utilities.h"
#include "coordinate.h"
#include "movement.h"
#include "board.h"

#include <list>
#include <memory> //get_pseudo_valid_movements of derived classes use shared_ptr<Piece>

namespace Chess {

    class Piece {
        protected:
            Coordinate coordinate;
            Chess::utilities::Color color;
            Chess::utilities::PieceType type;
            char symbol;
            bool had_moved;

        public:
            Piece(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type) :
                coordinate{coordinate}, 
                color{color}, 
                type{type}, 
                had_moved{false} {}
            virtual std::list<Movement> get_pseudo_valid_movements(Board& board) = 0;
            Coordinate get_coordinate() const;
            Chess::utilities::Color get_color() const;
            Chess::utilities::PieceType get_type() const;
            char get_symbol() const;
            bool get_had_moved() const;
    };
}

#endif