#include "Piece.h"

bool Chess::operator == (Piece& piece1, Piece& piece2) {
    return piece1.get_color() == piece2.get_color() &&
        piece1.get_coordinate() == piece2.get_coordinate() &&
        piece1.get_had_moved() == piece2.get_had_moved() &&
        piece1.get_symbol() == piece2.get_symbol() &&
        piece1.get_type() == piece2.get_type();
}