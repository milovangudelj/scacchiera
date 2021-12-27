#include "Board.h"
#include "pieces/King.h"

using Chess::KingPiece;

using Chess::Movement;
using Chess::utilities::Color;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;
using Chess::utilities::PieceType;

KingPiece::KingPiece(Coordinate coordinate, Color color, PieceType type) : Piece{coordinate, color, type}
{
    symbol = (color == Color::black) ? 'R' : 'r';
}

std::list<Movement> KingPiece::get_pseudo_valid_movements(Board &board)
{
    std::list<Movement> pseudo_movements;
    for (int i = 0; i < 8; i++)
    {
        std::shared_ptr<Piece> test_piece;
        Direction direction = static_cast<Direction>(i);
        std::pair<int, int> offset = DirectionOffset.at(direction);
        Coordinate test_coordinate = coordinate + offset;
        while (test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if (test_piece == nullptr)
            {
                pseudo_movements.push_back({coordinate, test_coordinate});
            }
            else
            {
                Color test_piece_color = test_piece->get_color();
                if (color != test_piece_color)
                {
                    pseudo_movements.push_back({coordinate, test_coordinate});
                }
                break;
            }
            test_coordinate = test_coordinate + offset;
        }
    }
    return pseudo_movements;
}