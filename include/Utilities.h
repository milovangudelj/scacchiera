/*Data structures shared by multiple classes
*/

#ifndef utilities_h
#define utilities_h

#include <ostream>
#include <string>
#include <map>
#include <utility>

namespace Chess::utilities {

    enum class Color {black, white};

    enum class Result {valid, invalid, check, checkmate, draw};

    enum class PlayerType {human, computer};

    enum class PieceType {king, queen, bishop, knight, rook, pawn};

    enum class Direction {left, up, right, down, left_up, right_up, right_down, left_down};

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

}

#endif