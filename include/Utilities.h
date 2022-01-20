/*Data structures shared by multiple classes
*/

#ifndef utilities_h
#define utilities_h

#include <map>
#include <utility>
#include <ostream>

namespace Chess::utilities {

    enum class Color {black, white};

    enum class Result {valid, invalid, check, checkmate, draw};

    enum class PlayerType {human, computer};

    enum class PieceType {king, queen, bishop, knight, rook, pawn};

    enum class Direction {left, up, right, down, left_up, right_up, right_down, left_down};

    enum class MoveResult {ok, invalid, promotion, check};

    extern std::map<Direction, std::pair<int, int>> DirectionOffset;

    std::ostream &operator<<(std::ostream &os, const Color &color);
    std::string operator+(std::string s, const Color &color);
    std::ostream &operator<<(std::ostream &os, const PlayerType &player);
}

#endif