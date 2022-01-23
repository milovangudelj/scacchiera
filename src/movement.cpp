/**
 * @file movement.cpp
 * @author Yihui Zhu
 * @brief Definition of Movement.h of Movement struct
 */

#include <iostream>

#include "Movement.h"

using Chess::Movement;

std::ostream& Chess::operator<<(std::ostream& os, Movement& movement) {
    os << movement.start << " " << movement.end;
    return os;
}