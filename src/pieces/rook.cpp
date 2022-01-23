// Tommaso Jin

#include "pieces/Rook.h"
#include "Board.h"

using Chess::Movement;
using Chess::Rook;
using Chess::utilities::Color;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;
using Chess::utilities::PieceType;

Rook::Rook(Coordinate coordinate, Color color, PieceType type) : Piece{coordinate, color, type}
{
	symbol = (color == Color::black) ? 'T' : 't';
}

/// @brief to get the list of possible movements
/// @param board reference to board
/// @returns list<Movement>
std::list<Movement> Rook::get_pseudo_valid_movements(Board &board)
{
	Coordinate test_coordinate;
	std::list<Movement> pseudo_movements;

	// rook can only move horizontally or vertically, through any number of unoccupied squares
	for (int i = 0; i < 4; i++)
	{
		Piece *test_piece;
		Direction direction = static_cast<Direction>(i);
		std::pair<int, int> offset = DirectionOffset.at(direction);

		test_coordinate = this->coordinate + offset;

		while (test_coordinate.is_valid())
		{
			test_piece = board.get_piece_at(test_coordinate);

			if (test_piece == nullptr)
			{
				pseudo_movements.push_back({this->coordinate, test_coordinate});
			}
			else
			{
				Color test_piece_color = test_piece->get_color();

				if (color != test_piece_color)
				{
					pseudo_movements.push_back({this->coordinate, test_coordinate});
				}

				break;
			}

			test_coordinate = test_coordinate + offset;
		}
	}

	return pseudo_movements;
}