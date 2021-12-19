#ifndef player_h
#define player_h

#include "Utilities.h"
#include "Piece.h"
#include <list>

using Chess::utilities::Color;
using Chess::utilities::PlayerType;

namespace Chess {

    class Player {
        private:
            PlayerType type;
            Color color;
            std::list<std::shared_ptr<Piece>> available_pieces;
            std::list<std::shared_ptr<Piece>> lost_pieces;
        
        public:
            const std::list<std::shared_ptr<Piece>>& get_available_pieces();
            const std::list<std::shared_ptr<Piece>>& get_lost_pieces();
            void add_to_lost_pieces(std::shared_ptr<Piece> piece);
            std::shared_ptr<Piece> remove_from_lost_pieces();
            PlayerType get_type() const;
            Color get_color() const;
    };
}

#endif