#ifndef PLAYER_H
#define PLAYER_H

#include "Utilities.h"
#include "Piece.h"

#include <list>
#include <memory>

namespace Chess {

    class Player {
        private:
            Chess::utilities::PlayerType type;
            Chess::utilities::Color color;
            std::list<std::shared_ptr<Piece>> available_pieces;
            std::list<std::shared_ptr<Piece>> lost_pieces;
        
        public:
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
    };
}

#endif // PLAYER_H