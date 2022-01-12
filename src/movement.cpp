#include "Movement.h"

using Chess::Movement;

std::ostream& Chess::operator<< (std::ostream& os, Movement& movement) {
    os << movement.start << " " << movement.end;
    return os;
}