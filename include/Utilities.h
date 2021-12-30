/*Data structures shared by multiple classes
*/

#ifndef utilities_h
#define utilities_h

#include <map>
#include <utility>

namespace Chess::utilities {

    enum class Color {black, white};

    enum class Result {valid, invalid, check, checkmate, draw};

    enum class PlayerType {human, computer};

    enum class PieceType {king, queen, bishop, knight, rook, pawn};

    enum class Direction {left, up, right, down, left_up, right_up, right_down, left_down};

    extern std::map<Direction, std::pair<int, int>> DirectionOffset;

}

#endif