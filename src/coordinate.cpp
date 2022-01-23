/**
 * @file coordinate.cpp
 * @author Yihui Zhu
 * @brief Definition file of Coordinate.h of Coordinate struct
 */

#include "Coordinate.h"
#include <string>
#include <map>

using Chess::Coordinate;

std::map<int, std::string> file_to_string = {
    {0, "A"},
    {1, "B"},
    {2, "C"},
    {3, "D"},
    {4, "E"},
    {5, "F"},
    {6, "G"},
    {7, "H"},
};

Coordinate Chess::operator+ (Coordinate coordinate, std::pair<int, int> offset) {
    coordinate.rank += offset.first;
    coordinate.file += offset.second;
    return coordinate;
}

std::string Chess::operator+ (std::string s, Coordinate coordinate) {
    std::string out;
    std::string file_character = file_to_string.at(coordinate.file);
    out += s + file_character + std::to_string(8 - coordinate.rank);
    return out;
}

bool Chess::operator== (const Coordinate &coordinate1, const Coordinate &coordinate2) {
    return coordinate1.rank == coordinate2.rank && coordinate1.file == coordinate2.file;
}

bool Chess::operator!=(const Coordinate &coordinate1, const Coordinate &coordinate2) {
    return coordinate1.rank != coordinate2.rank || coordinate1.file != coordinate2.file;
}

std::ostream &Chess::operator<<(std::ostream &os, Coordinate &coordinate) {
    std::string file_character = file_to_string.at(coordinate.file);
    os << file_character + std::to_string(8 - coordinate.rank);
    return os;
}