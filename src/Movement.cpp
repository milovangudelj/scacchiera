#include "movement.h"

using Chess::Movement;

std::ostream& operator<< (std::ostream& os, Movement& movement) {
    os << movement.start << " " << movement.end;
    return os;
}