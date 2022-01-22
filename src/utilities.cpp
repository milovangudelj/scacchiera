// Yihui Zhu

#include <ostream>

#include "Utilities.h"

using Chess::utilities::Color;
using Chess::utilities::Direction;
using Chess::utilities::PlayerType;

std::map<Direction, std::pair<int, int>> Chess::utilities::DirectionOffset {
    {Direction::left,       {0, -1}},
    {Direction::up,         {-1, 0}},
    {Direction::right,      {0, 1}},
    {Direction::down,       {1, 0}},
    {Direction::left_up,    {-1, -1}},
    {Direction::right_up,   {-1, 1}},
    {Direction::right_down, {1, 1}},
    {Direction::left_down,  {1, -1}}
};

namespace Chess::utilities
{
    std::ostream &operator<<(std::ostream &os, const Color &color)
    {
        os << ((color == Color::white) ? "white" : "black");
        return os;
    };

    std::string operator+(std::string s, const Color &color)
    {
        std::string out = s + ((color == Color::white) ? "white" : "black");
        return out;
    };

    std::ostream &operator<<(std::ostream &os, const PlayerType &type)
    {
        os << ((type == PlayerType::human) ? "human" : "computer");
        return os;
    };
}