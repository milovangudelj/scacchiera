#include "Coordinate.h"
#include <string>

using Chess::Coordinate;

Coordinate& Coordinate::operator+ (std::pair<int, int> offset) {
    auto [rank_offset, file_offset] = offset;
    rank += rank_offset;
    file += file_offset;
    return *this;
}

bool operator== (Coordinate& coordinate1, Coordinate& coordinate2) {
    return coordinate1.rank == coordinate2.rank && coordinate1.file == coordinate2.file;
}

std::ostream& operator<< (std::ostream& os, Coordinate& coordinate) {
    std::string file_character;
    switch (coordinate.file)
    {
    case 0:
        file_character = "A";
        break;
    case 1:
        file_character = "B";
        break;
    case 2:
        file_character = "C";
        break;
    case 3:
        file_character = "D";
        break;
    case 4:
        file_character = "E";
        break;
    case 5:
        file_character = "F";
        break;
    case 6:
        file_character = "G";
        break;
    case 7:
        file_character = "H";
        break;
    }
    os << file_character + std::to_string(coordinate.rank);
    return os;
}