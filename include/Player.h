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
            std::list<Piece*> available_pieces;
            std::list<Piece*> lost_pieces;
        
        public:
            const std::list<Piece*>& get_available_pieces();
            const std::list<Piece*>& get_lost_pieces();
            void add_to_lost_pieces(Piece* piece);
            Piece* remove_from_lost_pieces();
            PlayerType get_type() const;
            Color get_color() const;
    };
}

#endif