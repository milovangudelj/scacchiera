// Milovan Gudelj

#include "Board.h"
#include "pieces/Bishop.h"

using Chess::Bishop;

using Chess::Movement;
using Chess::utilities::Color;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;
using Chess::utilities::PieceType;

Bishop::Bishop(Coordinate coordinate, Color color, PieceType type) : Piece{coordinate, color, type}
{
	symbol = (color == Color::black) ? 'A' : 'a';
	update_cell_color();
}

std::list<Movement> Bishop::get_pseudo_valid_movements(Board &board)
{
	std::list<Movement> pseudo_movements;

	for (int i = 4; i < 8; i++) // Loop starts from 4 since the bishop can't move perpendicular to the board
	{
		Piece *test_piece;
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

Chess::utilities::Color Bishop::get_cell_color() const
{
	return cell_color;
};

void Bishop::update_cell_color()
{
	cell_color = (coordinate.rank + coordinate.file % 2 == 0) ? Color::white : Color::black;
};