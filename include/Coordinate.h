#ifndef coordinate_h
#define coordinate_h

#include <ostream>
#include <utility>

namespace Chess {

    struct Coordinate {
        unsigned int rank;
        unsigned int file;
        bool is_valid() const { return rank < 8 && file < 8; }
        Coordinate& operator+ (std::pair<int, int> offset);
    };

    bool operator== (Coordinate& coordinate1, Coordinate& coordinate2);

    std::ostream& operator<< (std::ostream& os, Coordinate& coordinate);
}

#endif