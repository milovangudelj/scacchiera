#ifndef PLAYER_H
#define PLAYER_H

#include "Utilities.h"
#include "Piece.h"

#include <list>
#include <memory>
#include <ostream>

namespace Chess {

    class Player {
        private:
            std::string name;
            Chess::utilities::PlayerType type;
            Chess::utilities::Color color;
            std::list<std::shared_ptr<Piece>> available_pieces;
            std::list<std::shared_ptr<Piece>> lost_pieces;
        
        public:
            // Constructors

            Player() = delete;

            /// @brief Constructs a Player object with the given color, type and name.
            /// @param _color The color of the player. Either 'white' or 'black'
            /// @param _type The type of the player. Either 'human' or 'computer'
            /// @param _name The name of the player.
            Player(const std::string &_color, const std::string &_type, const std::string &_name);

            /// @brief Constructs a Player object with the given color and type.
            /// @param _color The color of the player. Either 'white' or 'black'
            /// @param _type The type of the player. Either 'human' or 'computer'
            Player(const std::string &_color, const std::string &_type) : Player(_color, _type, ""){};

            /// @brief Constructs a 'human' Player object of the given color.
            /// @param _color The color of the player. Either 'white' or 'black'
            Player(const std::string &_color) : Player(_color, "human", ""){};

            // Simple inline getters

            const std::list<std::shared_ptr<Piece>> &get_available_pieces()
            {
                return available_pieces;
            };
            const std::list<std::shared_ptr<Piece>> &get_lost_pieces()
            {
                return lost_pieces;
            };
            Chess::utilities::PlayerType get_type() const
            {
                return type;
            };
            Chess::utilities::Color get_color() const
            {
                return color;
            };

            // Management member functions

            void add_to_lost_pieces(std::shared_ptr<Piece> &piece);
            void add_to_available_pieces(std::shared_ptr<Piece> &piece);
            std::shared_ptr<Piece> &remove_from_lost_pieces(std::shared_ptr<Piece> &piece);
            std::shared_ptr<Piece> &remove_from_available_pieces(std::shared_ptr<Piece> &piece);

            // Overloaded operators

            friend std::ostream &operator<<(std::ostream &os, const Player &player);
    };
}

#endif // PLAYER_H