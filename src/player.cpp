/**
 * @file player.cpp
 * @author Milovan Gudelj - Mat. 1192574
 * @brief Implementazione della classe Player contenente le informazioni relative ai giocatori.
 * @date 2022-22-01
*/

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

using namespace Chess;
using namespace Chess::utilities;

// Constructors

Player::Player(const Color &_color, const PlayerType &_type, const std::string &_name)
{
	type = _type;
	color = _color;
	name = (_name.length() == 0) ? ((_type == PlayerType::human) ? "Human" : "Hal") : _name;
	stale_since = 0;
};

// Player management member functions

bool exists(std::unique_ptr<Chess::Piece> &_lost_piece, Chess::Piece *_piece)
{
	return _lost_piece.get() == _piece;
}

void Player::move_to_lost_pieces(Piece *_piece)
{
	std::list<std::unique_ptr<Chess::Piece>>::iterator piece_it;
	piece_it = std::find_if(available_pieces.begin(), available_pieces.end(), [&_piece](auto &available_piece)
									{ return available_piece.get() == _piece; });

	lost_pieces.push_back(std::move(*piece_it));
	lost_pieces_copy.push_back(lost_pieces.back().get());

	available_pieces.erase(piece_it);
	available_pieces_copy.remove(lost_pieces.back().get());
};

void Player::move_to_available_pieces(Piece *_piece)
{
	std::list<std::unique_ptr<Chess::Piece>>::iterator piece_it;
	piece_it = std::find_if(lost_pieces.begin(), lost_pieces.end(), [&_piece](auto &lost_piece)
									{ return lost_piece.get() == _piece; });

	available_pieces.push_back(std::move(*piece_it));
	available_pieces_copy.push_back(available_pieces.back().get());

	lost_pieces.erase(piece_it);
	lost_pieces_copy.remove(available_pieces.back().get());
};

Piece *Player::add_to_available_pieces(Coordinate _coordinate, Color _color, PieceType _type)
{
	switch (_type)
	{
	case PieceType::king:
		available_pieces.push_back(std::make_unique<King>(_coordinate, _color, _type));
		break;
	case PieceType::queen:
		available_pieces.push_back(std::make_unique<Queen>(_coordinate, _color, _type));
		break;
	case PieceType::bishop:
		available_pieces.push_back(std::make_unique<Bishop>(_coordinate, _color, _type));
		break;
	case PieceType::knight:
		available_pieces.push_back(std::make_unique<Knight>(_coordinate, _color, _type));
		break;
	case PieceType::rook:
		available_pieces.push_back(std::make_unique<Rook>(_coordinate, _color, _type));
		break;
	case PieceType::pawn:
		available_pieces.push_back(std::make_unique<Pawn>(_coordinate, _color, _type));
		break;
	}

	Piece *piece = available_pieces.back().get();
	available_pieces_copy.push_back(piece);

	return piece;
}

// Overloaded operators

namespace Chess
{
	std::ostream &operator<<(std::ostream &_os, const Player &_player)
	{
		_os << "Player:\n _name: " << _player.name << "\n _color: " << _player.color << "\n _type: " << _player.type;
		return _os;
	};
}