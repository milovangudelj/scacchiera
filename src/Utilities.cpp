#include "Utilities.h"

using Chess::utilities::Direction;

std::map<Direction, std::pair<int, int>> DirectionOffset {
    {Direction::left,       {0, -1}},
    {Direction::up,         {1, 0}},
    {Direction::right,      {0, 1}},
    {Direction::down,       {-1, 0}},
    {Direction::left_up,    {1, -1}},
    {Direction::right_up,   {1, 1}},
    {Direction::right_down, {-1, 1}},
    {Direction::left_down,  {-1, -1}}
};