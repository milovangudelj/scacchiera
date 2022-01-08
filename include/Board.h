#ifndef board_h
#define board_h

#include <array>
#include <memory>
#include <string>
#include <ostream>
#include <map>

#include "Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Bishop.h"
#include "pieces/Knight.h"
#include "pieces/Rook.h"
#include "pieces/Pawn.h"
#include "Coordinate.h"
#include "Player.h"
#include "Utilities.h"

template<typename T, unsigned int SIZE> 
using Matrix = std::array<std::array<T, SIZE>, SIZE>;

constexpr int SIZE = 8;

namespace Chess {

    class Board {
        private:
            Matrix<std::shared_ptr<Piece>, SIZE> cells;
            void initialize_with_fen(std::string fen, Player& player1, Player& player2);
            Coordinate w_king_coordinate;
            Coordinate b_king_coordinate;
            std::shared_ptr<Piece> last_eaten;
            Movement last_movement;
            bool is_check(Player& current, Player& other);
            void temporary_move(Movement movement); //modifies last_eaten
            Chess::utilities::MoveResult handle_castling(Player& current_player, Player& other_player, Movement movement);
            void handle_en_passant(Player& current_player, Player& other_player, Movement movement);
            void undo(Movement previous_movement, std::shared_ptr<Piece> previous_eaten);
            std::map<std::string, int> position_history;
            std::string to_fen();
            bool can_draw_flag;
        public:
            Board(std::string fen, Player& player1, Player& player2);
            std::shared_ptr<Piece> get_piece_at(Coordinate coordinate) const;
            friend std::ostream& operator<< (std::ostream& os, const Board& board);
            Chess::utilities::MoveResult move(Player& current_player, Player& other_player, Movement movement);
            bool promote(Player& player, char piece_symbol);
            Movement get_last_movement() const { return last_movement; }
            bool is_checkmate(Player& current, Player& other);
            bool is_draw(Player& current, Player& other);
            bool can_draw(); //threefold repetition
    };

}

#endif
