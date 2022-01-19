#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>
#include <ostream>

#include "Utilities.h"
#include "Coordinate.h"
#include "Piece.h"

namespace Chess
{
    class Player
    {
    private:
        std::string name;
        Chess::utilities::PlayerType type;
        Chess::utilities::Color color;
        std::list<std::unique_ptr<Piece>> available_pieces;
        std::list<std::unique_ptr<Piece>> lost_pieces;
        std::list<Piece *> available_pieces_copy;
        std::list<Piece *> lost_pieces_copy;
        int stale_since;

    public:
        // Constructors

        /// @brief Constructs a Player object with the given color, type and name.
        /// @param _color The color of the player. Either 'white' or 'black'
        /// @param _type The type of the player. Either 'human' or 'computer'
        /// @param _name The name of the player.
        Player(const Chess::utilities::Color &_color, const Chess::utilities::PlayerType &_type, const std::string &_name);

        /// @brief Constructs a Player object with the given color and type.
        /// @param _color The color of the player. Either 'white' or 'black'
        /// @param _type The type of the player. Either 'human' or 'computer'
        Player(const Chess::utilities::Color &_color, const Chess::utilities::PlayerType &_type) : Player(_color, _type, ""){};

        /// @brief Constructs a 'human' Player object of the given color.
        /// @param _color The color of the player. Either 'white' or 'black'
        Player(const Chess::utilities::Color &_color) : Player(_color, Chess::utilities::PlayerType::human){};

        Player() : Player(Chess::utilities::Color::white){};

        // Simple inline getters

        const std::list<Piece *> &get_available_pieces()
        {
            return available_pieces_copy;
        };
        const std::list<Piece *> &get_lost_pieces()
        {
            return lost_pieces_copy;
        };
        Chess::utilities::PlayerType get_type() const
        {
            return type;
        };
        Chess::utilities::Color get_color() const
        {
            return color;
        };
        std::string get_name() const
        {
            return name;
        }
        int get_stale_since() const { return stale_since; };

        // Management member functions

        void move_to_lost_pieces(Piece *piece);
        void move_to_available_pieces(Piece *piece);
        Piece *add_to_available_pieces(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
        void increment_stale_since() { stale_since++; };
        void reset_stale_since() { stale_since = 0; };

        // Overloaded operators

        friend std::ostream &operator<<(std::ostream &os, const Player &player);
    };

} // namespace Chess

#endif // PLAYER_H