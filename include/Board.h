#ifndef board_h
#define board_h

#include <array>
#include <memory>
#include <string>
#include <ostream>

#include "Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Bishop.h"
#include "pieces/Knight.h"
#include "pieces/Rook.h"
#include "Coordinate.h"
#include "Player.h"
#include "Utilities.h"

template<typename T, unsigned int SIZE> 
using Matrix = std::array<std::array<T, SIZE>, SIZE>;

namespace Chess {

    class Board {
        private:
            Matrix<std::shared_ptr<Piece>, 8> cells;
            void initialize_with_fen(std::string fen); //TODO exceptions
            Coordinate w_king_coordinate;
            Coordinate b_king_coordinate;
            std::shared_ptr<Piece> last_eaten;
            bool is_check(Player& current, Player& other, Board& board);
        public:
            Board(std::string fen);
            std::shared_ptr<Piece> get_piece_at(Coordinate coordinate);
            friend std::ostream& operator<< (std::ostream& os, const Board& board);
    };

    std::shared_ptr<Piece> make_piece(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);

}

#endif
