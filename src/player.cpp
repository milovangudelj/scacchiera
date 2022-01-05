#include "Player.h"
#include "Utilities.h"
#include "Piece.h"

#include <list>
#include <memory>

using Chess::Piece;
using Chess::Player;

void Player::add_to_lost_pieces(std::shared_ptr<Piece> &piece)
{
	// Add piece to the back of the list
	lost_pieces.push_back(piece);
};

void Player::add_to_available_pieces(std::shared_ptr<Piece> &piece)
{
	// Add piece to the back of the list
	available_pieces.push_back(piece);
};

std::shared_ptr<Piece> &Player::remove_from_lost_pieces(std::shared_ptr<Piece> &piece)
{
	// Removing the given piece from the list
	lost_pieces.remove(piece);

	return piece;
};

std::shared_ptr<Piece> &Player::remove_from_available_pieces(std::shared_ptr<Piece> &piece)
{
	// Removing the given piece from the list
	available_pieces.remove(piece);

	return piece;
};