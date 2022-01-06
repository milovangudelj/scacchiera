#include "Player.h"
#include "Utilities.h"
#include "Piece.h"

#include <list>
#include <memory>
#include <ostream>

using Chess::Piece;
using Chess::Player;
using Chess::utilities::Color;
using Chess::utilities::PlayerType;

// Constructors

Player::Player(const std::string &_color, const std::string &_type, const std::string &_name)
{
	if (_color.compare("white") != 0 && _color.compare("black") != 0)
	{
		throw "Invalid player color";
	}

	if (_type.compare("human") != 0 && _type.compare("computer") != 0)
	{
		throw "Invalid player type";
	}

	type = _type.compare("human") == 0 ? PlayerType::human : PlayerType::computer;
	color = _color.compare("white") == 0 ? Color::white : Color::black;
	name = (_name.length() == 0) ? ((type == PlayerType::human) ? "Human" : "Hal") : _name;
	stale_since = 0;
};

// Management methods

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

// Overloaded operators

namespace Chess
{
	std::ostream &operator<<(std::ostream &os, const Player &player)
	{
		os << "Player:\n _name: " << player.name << "\n _color: " << player.color << "\n _type: " << player.type;
		return os;
	};
}