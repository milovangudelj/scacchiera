#ifndef board_h
#define board_h

#include <array>
#include <memory>
#include <string>
#include <ostream>
#include <map>
#include <initializer_list>

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
    protected:
        std::shared_ptr<Chess::Player> player1;
        std::shared_ptr<Chess::Player> player2;

    private:
        Matrix<std::shared_ptr<Piece>, SIZE> cells;
        void initialize_with_fen(std::string fen, std::initializer_list<bool> had_moved_flags, Player &player1, Player &player2);
        void from_fen(std::string fen);

        //makes checking for check condition easier and faster
        Coordinate w_king_coordinate;
        Coordinate b_king_coordinate;

        //needed for undoing invalid moves
        std::shared_ptr<Piece> last_eaten;
        Movement last_movement;
        void undo(Movement previous_movement, std::shared_ptr<Piece> previous_eaten);

        //side effect: modifies last_eaten and last_movement
        void temporary_move(Movement movement);

        bool is_check(Player &current, Player &other);
        Chess::utilities::MoveResult handle_castling(Player &current_player, Player &other_player, Movement movement);
        void handle_en_passant(Player &current_player, Player &other_player, Movement movement);

        //needed by can_draw for threefold repetition
        std::map<std::string, int> position_history;
        std::string to_fen(Chess::utilities::Color current_color); //takes snapshot of current board position
        bool can_draw_flag;

    public:
        Board(std::string fen, std::shared_ptr<Player> p1, std::shared_ptr<Player> p2);
        Chess::utilities::MoveResult move(Player &current_player, Player &other_player, Movement movement);
        bool promote(Player &player, char piece_symbol);

        bool is_checkmate(Player &current, Player &other);
        bool is_draw(Player &current, Player &other); //stalemate, dead position and 50 moves rule
        bool can_draw();                              //returns true if threefold repetition occured

        //used by Pawn for checking for en passant conditions
        Movement get_last_movement() const { return last_movement; }

        std::shared_ptr<Piece> get_piece_at(Coordinate coordinate) const;
        std::string pretty_print();
        friend std::ostream &operator<<(std::ostream &os, const Board &board);
    };

}

#endif
