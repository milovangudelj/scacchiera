#include "King_Piece.h"

using Chess::KingPiece;
using Chess::Movement;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;

KingPiece::KingPiece(Color color, Coordinate coordinate, PieceType type) : Piece{color, coordinate, type} {
    symbol = (color == Color::black) ? 'R' : 'r';
}

std::list<Movement>& KingPiece::get_pseudo_valid_movements(Board& board) {
    std::list<Movement> pseudo_movements;
    for (int i = 0; i < 8; i++) {
        std::shared_ptr<Piece> test_piece;
        Direction direction = static_cast<Direction>(i);
        std::pair<int, int> offset = DirectionOffset.at(direction);
        Coordinate test_coordinate = coordinate + offset;
        while(test_coordinate.is_valid()) {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr) {
                pseudo_movements.push_back({coordinate, test_coordinate});
            } else {
                Color test_piece_color = test_piece->get_color();
                if(color != test_piece_color) {
                    pseudo_movements.push_back({coordinate, test_coordinate});
                }
                break;
            }
            test_coordinate = test_coordinate + offset;
        }
    }
    return pseudo_movements;
}