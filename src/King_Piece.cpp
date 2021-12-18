#include "King_Piece.h"

using Chess::KingPiece;
using Chess::Movement;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;

KingPiece::KingPiece(Color color, Coordinate coordinate, PieceType type) : Piece{color, coordinate, type} {
    symbol = (color == Color::black) ? 'R' : 'r';
}

std::list<Movement> KingPiece::get_pseudo_valid_movements(Board& board) {
    std::list<Movement> pseudo_movements;
    for (int i = 0; i < 8; i++) {
        Piece* test_piece;
        Direction direction = static_cast<Direction>(i);
        std::pair offset = DirectionOffset.at(direction);
        Coordinate test_coordinate = coordinate;
        do {
            test_coordinate = test_coordinate + offset;
            if(!test_coordinate.is_valid()) {
                break;
            }
            test_piece = board.get_piece_at(test_coordinate);
            pseudo_movements.push_back({coordinate, test_coordinate});
        } while(test_piece == nullptr);
    }
    return pseudo_movements;
}