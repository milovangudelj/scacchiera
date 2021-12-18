#ifndef movement_h
#define movement_h

#include "Coordinate.h"

using Chess::Coordinate;

namespace Chess {
    
    struct Movement {
        Coordinate start;
        Coordinate end;
        bool is_promotion;
        bool is_en_passant;
        bool is_castling;
    };

    std::ostream& operator<< (std::ostream& os, Movement& movement);
}


#endif