#include <list>
#include <memory>
#include <ostream>
#include <algorithm>

#include "Player.h"
#include "Utilities.h"
#include "Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Bishop.h"
#include "pieces/Knight.h"
#include "pieces/Rook.h"
#include "pieces/Pawn.h"

using Chess::Piece;
using Chess::King;
using Chess::Queen;
using Chess::Bishop;
using Chess::Knight;
using Chess::Rook;
using Chess::Pawn;
using Chess::Player;
using Chess::Coordinate;
using Chess::utilities::Color;
using Chess::utilities::PlayerType;
using Chess::utilities::PieceType;

// Constructors

Player::Player(const Color &_color, const PlayerType &_type, const std::string &_name)
{
	type = _type;
	color = _color;
	name = (_name.length() == 0) ? ((_type == PlayerType::human) ? "Human" : "Hal") : _name;
	stale_since = 0;
};

// Management methods

void Player::move_to_lost_pieces(Piece *piece)
{
	auto piece_it = std::find_if(available_pieces.begin(), available_pieces.end(), 
		[piece] (auto& lost_piece) {
			return lost_piece.get() == piece;
	});
	lost_pieces.push_back(std::move(*piece_it));
	available_pieces.erase(piece_it);
	available_pieces_copy.remove(lost_pieces.back().get());
	lost_pieces_copy.push_back(lost_pieces.back().get());
};

void Player::move_to_available_pieces(Piece *piece)
{
	auto piece_it = std::find_if(lost_pieces.begin(), lost_pieces.end(), 
		[piece] (auto& lost_piece) {
			return lost_piece.get() == piece;
	});
	available_pieces.push_back(std::move(*piece_it));
	lost_pieces.erase(piece_it);
	lost_pieces_copy.remove(available_pieces.back().get());
	available_pieces_copy.push_back(available_pieces.back().get());
};

Piece *Player::add_to_available_pieces(Coordinate coordinate, Color color, PieceType type) {
	switch (type)
	{
	case PieceType::king:
		available_pieces.push_back(std::make_unique<King>(coordinate, color, type));
		break;
	case PieceType::queen:
		available_pieces.push_back(std::make_unique<Queen>(coordinate, color, type));
		break;
	case PieceType::bishop:
		available_pieces.push_back(std::make_unique<Bishop>(coordinate, color, type));
		break;
	case PieceType::knight:
		available_pieces.push_back(std::make_unique<Knight>(coordinate, color, type));
		break;
	case PieceType::rook:
		available_pieces.push_back(std::make_unique<Rook>(coordinate, color, type));
		break;
	case PieceType::pawn:
		available_pieces.push_back(std::make_unique<Pawn>(coordinate, color, type));
		break;
	}
	Piece *piece = available_pieces.back().get();
	available_pieces_copy.push_back(piece);
	return piece;
}

// Overloaded operators

namespace Chess
{
	std::ostream &operator<<(std::ostream &os, const Player &player)
	{
		os << "Player:\n _name: " << player.name << "\n _color: " << player.color << "\n _type: " << player.type;
		return os;
	};
}