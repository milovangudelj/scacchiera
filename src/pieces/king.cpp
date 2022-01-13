#include "Board.h"
#include "pieces/King.h"

using Chess::King;

using Chess::Movement;
using Chess::Coordinate;
using Chess::utilities::Color;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;
using Chess::utilities::PieceType;

Coordinate get_testing_coordinate(Coordinate starting_coordinate, Chess::utilities::Direction direction) {
    return starting_coordinate + Chess::utilities::DirectionOffset.at(direction);
}

King::King(Coordinate coordinate, Color color, PieceType type) : Piece{coordinate, color, type}
{
    symbol = (color == Color::black) ? 'R' : 'r';
}

std::list<Movement> King::get_pseudo_valid_movements(Board &board)
{
    std::list<Movement> pseudo_movements;

    Direction direction;
    std::shared_ptr<Piece> test_piece;
    Coordinate test_coordinate;

    //looks for all 8 adjacent positions
    for (int i = 0; i < 8; i++) {
        direction = static_cast<Direction>(i);
        test_coordinate = get_testing_coordinate(this->coordinate, direction);
        if(test_coordinate.is_valid()) {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr || test_piece->get_color() != this->color) {
                pseudo_movements.push_back({this->coordinate, test_coordinate});
            }
        }
    }

    //check for castling conditions
    if(!this->get_had_moved()) {
        //short castling
        int i = 2;
        test_coordinate = this->coordinate;
        do {
            test_coordinate = get_testing_coordinate(test_coordinate, Direction::right);
            test_piece = board.get_piece_at(test_coordinate);
            //castling allowed when there are no other pieces between king and rook
            if(test_piece != nullptr) {
                break;
            }
            i--;
        } while (i > 0);
        test_coordinate = get_testing_coordinate(test_coordinate, Direction::right);
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece != nullptr && test_piece->get_type() == PieceType::rook && test_piece->get_color() == this->get_color() && test_piece->get_had_moved() == false) {
            pseudo_movements.push_back({this->coordinate, test_coordinate, false, false, false, true});
        }

        //long castling
        i = 3;
        test_coordinate = this->coordinate;
        do {
            test_coordinate = get_testing_coordinate(test_coordinate, Direction::left);
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr) {
                break;
            }
            i--;
        } while(i > 0);
        test_coordinate = get_testing_coordinate(test_coordinate, Direction::left);
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece != nullptr && test_piece->get_type() == PieceType::rook && test_piece->get_color() == this->get_color() && test_piece->get_had_moved() == false) {
            pseudo_movements.push_back({this->coordinate, test_coordinate, false, false, true, false});
        }
    }

    return pseudo_movements;
}