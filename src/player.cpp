#include "Player.h"
#include "Utilities.h"
#include "Piece.h"

#include <list>
#include <memory>

using Chess::Piece;
using Chess::Player;

void Player::add_to_lost_pieces(std::shared_ptr<Piece> &piece)
{
	lost_pieces.push_back(piece);
	available_pieces.remove(piece);
};

std::shared_ptr<Piece> &Player::remove_from_lost_pieces()
{
	// Removing the last one in the list
	std::shared_ptr<Piece> removed_piece = lost_pieces.back();

	available_pieces.push_back(removed_piece);
	lost_pieces.pop_back();

	return removed_piece;
};