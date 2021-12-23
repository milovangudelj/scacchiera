#include "king_piece.h"

using Chess::KingPiece;

using Chess::Movement;
using Chess::Coordinate;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;
using Chess::utilities::Color;
using Chess::utilities::PieceType;

KingPiece::KingPiece(Coordinate coordinate, Color color, PieceType type) : Piece{coordinate, color, type} {
    symbol = (color == Color::black) ? 'R' : 'r';
}

std::list<Movement> KingPiece::get_pseudo_valid_movements(Board& board) {
    std::list<Movement> pseudo_movements;

    Direction direction;
    std::shared_ptr<Piece> test_piece;
    Coordinate test_coordinate;

    //looks for all 8 adjacent positions
    for (int i = 0; i < 8; i++) {
        direction = static_cast<Direction>(i);
        test_coordinate = get_testing_coordinate(this->coordinate, direction);
        while(test_coordinate.is_valid()) {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr) {
                pseudo_movements.push_back({this->coordinate, test_coordinate});
            } else {
                if(this->color != test_piece->get_color()) {
                    pseudo_movements.push_back({this->coordinate, test_coordinate});
                }
                break;
            }
            test_coordinate = get_testing_coordinate(test_coordinate, direction);
        }
    }

    //check for castling conditions
    if(!had_moved()) {
        //short castling
        int i = 2;
        test_coordinate = this->coordinate;
        do {
            test_coordinate = get_testing_coordinate(test_coordinate, Direction::right);
            test_piece = board.get_piece_at(test_coordinate);
            //castling allowed when there are no other pieces between king and rook
            if(test_piece != nullptr) {
                return pseudo_movements;
            }
            i--;
        } while (i > 0);
        test_coordinate = get_testing_coordinate(test_coordinate, Direction::right);
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece->get_type() == PieceType::rook && test_piece->had_moved() == false) {
            //is castling is allowed then the king can move two positions to right/left
            Coordinate end_coordinate = get_testing_coordinate(get_testing_coordinate(this->coordinate, Direction::right), Direction::right);
            pseudo_movements.push_back({this->coordinate, end_coordinate, false, false, true});
        }

        //long castling
        i = 3;
        test_coordinate = this->coordinate;
        do {
            test_coordinate = get_testing_coordinate(test_coordinate, Direction::left);
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr) {
                return pseudo_movements;
            }
            i--;
        } while(i > 0);
        test_coordinate = get_testing_coordinate(test_coordinate, Direction::left);
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece->get_type() == PieceType::rook && test_piece->had_moved() == false) {
            Coordinate end_coordinate = get_testing_coordinate(get_testing_coordinate(this->coordinate, Direction::left), Direction::left);
            pseudo_movements.push_back({this->coordinate, end_coordinate, false, false, true});
        }
    }

    return pseudo_movements;
}