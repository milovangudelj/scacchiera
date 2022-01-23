/**
 * @file Piece.h
 * @author Yihui Zhu
 * @brief Header file of Piece class
 */

#ifndef piece_h
#define piece_h

#include "Utilities.h"
#include "Coordinate.h"
#include "Movement.h"

#include <list>

namespace Chess {

    class Board; //predeclaration to avoid circular dependency

    class Piece {

        protected:

            Coordinate coordinate;
            Chess::utilities::Color color;
            Chess::utilities::PieceType type;
            char symbol;
            bool had_moved;
            bool selected;

        public:
            Piece(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type) : 
                coordinate{coordinate}, color{color}, type{type}, had_moved{false}, selected{false} {}

            /**
             * @brief Returns all possible moves of the piece without caring if it can cause a check
             * @param board Need for invoking its get_cell_at() method
             * @return std::list<Movement> 
             */
            virtual std::list<Movement> get_pseudo_valid_movements(const Board& board) = 0;
            
            Coordinate get_coordinate() const { return coordinate;};
            Chess::utilities::Color get_color() const { return color;};
            Chess::utilities::PieceType get_type() const { return type;};
            char get_symbol() const { return symbol;};
            bool get_had_moved() const { return had_moved;};
            bool is_selected() const { return selected; };
            void set_selected(bool val = true) { selected = val; };
            void set_had_moved(bool val = true) { had_moved = true; };
            void set_coordinate(Coordinate coordinate) { this->coordinate = coordinate; }
    };

    bool operator == (Piece& piece1, Piece& piece2);
}

#endif