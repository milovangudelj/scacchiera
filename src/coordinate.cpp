#include "Coordinate.h"
#include <string>

using Chess::Coordinate;

Coordinate Chess::operator+(Coordinate coordinate, std::pair<int, int> offset)
{
    coordinate.rank += offset.first;
    coordinate.file += offset.second;
    return coordinate;
}

std::string Chess::operator+(std::string s, Coordinate coordinate)
{
    std::string out;
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
    out += s + file_character + std::to_string(8 - coordinate.rank);
    return out;
}

bool Chess::operator==(const Coordinate &coordinate1, const Coordinate &coordinate2)
{
    return coordinate1.rank == coordinate2.rank && coordinate1.file == coordinate2.file;
}

bool Chess::operator!=(const Coordinate &coordinate1, const Coordinate &coordinate2)
{
    return coordinate1.rank != coordinate2.rank || coordinate1.file != coordinate2.file;
}

std::ostream &Chess::operator<<(std::ostream &os, Coordinate &coordinate)
{
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
    os << file_character + std::to_string(8 - coordinate.rank);
    return os;
}