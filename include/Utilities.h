/*Data structures shared by multiple classes
*/

#ifndef utilities_h
#define utilities_h

#include <ostream>
#include <string>
#include <map>
#include <utility>

namespace Chess::utilities {

    //Coordinate

    struct Coordinate {
    unsigned int rank : 3;
    unsigned int file : 3;
    };

    Coordinate& operator+ (Coordinate& coordinate, std::pair<int, int> offset) {
        auto [rank_offset, file_offset] = offset;
        return {coordinate.rank + rank_offset, coordinate.file + file_offset};

    }

    bool operator== (Coordinate& coordinate1, Coordinate& coordinate2) {
        return coordinate1.rank == coordinate2.rank && coordinate1.file == coordinate2.file;
    }

    std::ostream& operator<< (std::ostream& os, Coordinate coordinate) {
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

    //end Coordinate

    //Direction and DirectionOffset

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

    //end Direction and DirectionOffset

    enum class Color {black, white};

    enum class Result {valid, invalid, check, checkmate, draw};

    enum class PlayerType {human, computer};

}



#endif