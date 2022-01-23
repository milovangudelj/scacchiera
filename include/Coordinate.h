/**
 * @file Coordinate.h
 * @author Yihui Zhu
 * @brief Header file of Coordinate struct
 */

#ifndef coordinate_h
#define coordinate_h

#include <ostream>
#include <utility>

namespace Chess {

    struct Coordinate {
        unsigned int rank;
        unsigned int file;
        bool is_valid() const { return rank < 8 && file < 8; }
    };

    Coordinate operator+ (Coordinate coordinate, std::pair<int, int> offset);

    std::string operator+ (std::string s, Coordinate coordinate);

    bool operator== (const Coordinate& coordinate1, const Coordinate& coordinate2);
    bool operator!=(const Coordinate &coordinate1, const Coordinate &coordinate2);

    std::ostream& operator<< (std::ostream& os, Coordinate& coordinate);
}

#endif