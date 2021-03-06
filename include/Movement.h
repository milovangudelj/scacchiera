/**
 * @file Movement.h
 * @author Yihui Zhu
 * @brief Header file of Movement struct
 */

#ifndef movement_h
#define movement_h

#include "Coordinate.h"

#include <ostream>

namespace Chess {
    
    struct Movement {
        Chess::Coordinate start;
        Chess::Coordinate end;
        bool is_promotion;
        bool is_en_passant;
        bool is_long_castling;
        bool is_short_castling;
    };

    std::ostream& operator<< (std::ostream& os, Movement& movement);
}


#endif