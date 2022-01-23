/**
 * @file bishop.cpp
 * @author Milovan Gudelj - Mat. 1192574
 * @brief Implementazione della classe Bishop che gestisce il pezzo dell'alfiere.
 * @date 2022-22-01
*/

#include "Board.h"
#include "pieces/Bishop.h"

using namespace Chess;
using namespace Chess::utilities;

// Constructors

Bishop::Bishop(Coordinate _coordinate, Color _color, PieceType _type) : Piece{_coordinate, _color, _type}
{
	symbol = (_color == Color::black) ? 'A' : 'a';
	update_cell_color();
}

// Getters

std::list<Movement> Bishop::get_pseudo_valid_movements(const Board &_board)
{
	std::list<Movement> pseudo_movements;

	for (int i = 4; i < 8; i++) // Loop starts from 4 since the bishop can't move perpendicular to the board
	{
		Piece *test_piece;

		// Direction in which to look for valid movements
		Direction direction = static_cast<Direction>(i);

		// Direction offset. For finding the coordinates to check for validity
		std::pair<int, int> offset = DirectionOffset.at(direction);

		Coordinate test_coordinate = coordinate + offset;

		while (test_coordinate.is_valid())
		{
			test_piece = _board.get_piece_at(test_coordinate);

			// If test_coordinate points to an empty cell, it gets added to the list
			if (test_piece == nullptr)
			{
				pseudo_movements.push_back({coordinate, test_coordinate});
			}
			else
			{
				Color test_piece_color = test_piece->get_color();

				// If the cell is occupied by an opponent's piece, it gets added to the list
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

// Utility

void Bishop::update_cell_color()
{
	cell_color = (coordinate.rank + coordinate.file % 2 == 0) ? Color::white : Color::black;
};