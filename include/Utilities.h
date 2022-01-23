/**
 * @file Utilities.h
 * @author Yihui Zhu
 * @brief Header of enums and helper structures shared by multiple classes
*/

#ifndef utilities_h
#define utilities_h

#include <map>
#include <utility>
#include <ostream>

/**
 * @brief Enums and data structures shared by multple classes
 */
namespace Chess::utilities {

    enum class Color {black, white};

    enum class Result {valid, invalid, check, checkmate, draw};

    enum class PlayerType {human, computer};

    enum class PieceType {king, queen, bishop, knight, rook, pawn};

    enum class Direction {left, up, right, down, left_up, right_up, right_down, left_down};

    enum class MoveResult {ok, invalid, promotion, check};

    /**
     * @brief example: Coordinate b = a + DirectionOffset.at(Direction::left);
     */
    extern std::map<Direction, std::pair<int, int>> DirectionOffset;

    std::ostream& operator<< (std::ostream& os, const Color& color);
    std::ostream& operator<< (std::ostream& os, const PlayerType& player);
    std::string operator+ (std::string s, const Color &color);
}

#endif