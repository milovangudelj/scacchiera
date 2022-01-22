// Yihui Zhu

#ifndef board_h
#define board_h

#include <array>
#include <string>
#include <ostream>
#include <map>
#include <initializer_list>

#include "Piece.h"
#include "pieces/Bishop.h"
#include "Coordinate.h"
#include "Player.h"
#include "Utilities.h"

template<typename T, unsigned int SIZE> 
using Matrix = std::array<std::array<T, SIZE>, SIZE>;

constexpr int SIZE = 8;

namespace Chess {

    class Board {
    protected:
        Chess::Player *player1;
        Chess::Player *player2;
        Chess::Piece *selected_piece;

    private:
        Matrix<Piece *, SIZE> cells{};
        void initialize_with_fen(std::string fen, std::initializer_list<bool> had_moved_flags, Player &player1, Player &player2);
        void from_fen(std::string fen);

        //makes checking for check condition easier and faster
        Piece *w_king;
        Piece *b_king;

        //needed for undoing invalid moves
        Piece *last_eaten;
        Movement last_movement;
        void undo(Movement previous_movement, Piece *previous_eaten);

        //side effect: modifies last_eaten and last_movement
        void temporary_move(Movement movement);

        bool is_check(Player &current, Player &other);
        Chess::utilities::MoveResult handle_castling(Player &current_player, Player &other_player, Movement movement);
        void handle_en_passant(Player &current_player, Player &other_player, Movement movement);

        //needed by can_draw for threefold repetition
        std::map<std::string, int> position_history;
        std::string to_fen(Chess::utilities::Color current_color); //takes snapshot of current board position

    public:
        Board(std::string fen, Player *p1, Player *p2);
        Chess::utilities::MoveResult move(Player &current_player, Player &other_player, Movement movement);
        void promote(Player &player, char piece_symbol);

        bool is_checkmate(Player &current, Player &other);
        bool is_draw(Player &current, Player &other); //stalemate, dead position and 50 moves rule

        //used by Pawn for checking for en passant conditions
        Movement get_last_movement() const { return last_movement; }

        Piece *get_piece_at(Coordinate coordinate) const;
        std::string pretty_print();
        friend std::ostream &operator<<(std::ostream &os, const Board &board);
    };

}

#endif
