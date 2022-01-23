/**
 * @file Board.h
 * @author Yihui Zhu
 * @brief Header file for Board class
 */

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

/**
 * @brief Represents a 2d array
 * @tparam T 
 * @tparam SIZE Both horizontal and vertical size
 */
template<typename T, unsigned int SIZE> 
using Matrix = std::array<std::array<T, SIZE>, SIZE>;

constexpr int SIZE = 8;

namespace Chess {

    class Board {

        protected:

            Chess::Piece* selected_piece; //the selected piece will be displayed with a highlight color

        private:
            Matrix<Piece*, SIZE> cells{};

            /**
             * @brief initialized cells with piece by parsing the fen input
             * @param fen String in fen standard
             * @param player1 player to which the pieces are assigned based on the color
             * @param player2 player to which the pieces are assigned based on the color
             */
            void from_fen(std::string fen, Chess::Player* player1, Chess::Player* player2);

            Piece* w_king; //Makes checking for checks easier and faster
            Piece* b_king;

            Piece* last_eaten; //Needed for undoing invalid moves
            Movement last_movement;

            /**
             * @brief Used only when last_movement has resulted in check
             * @param previous_movement The movement previous to last_movement, have to copy it before temporary_move()
             * @param previous_eaten The piece eaten previous to last_eaten, have to copy it before temporary_move()
             */
            void undo(Movement previous_movement, Piece* previous_eaten);

            
            /**
             * @brief Moves piece in cells according to parameter
             * Side effect: modifies last_movement and last_eaten
             * @param movement Where from and where to move
             */
            void temporary_move(Movement movement);

            /**
             * @brief Returns true if player's king is in check
             * @param current The player you want to check checking condition of
             * @param other The adversary, which pieces might check the opponent's king
             * @return true 
             * @return false 
             */
            bool is_check(Player& current, Player& other);

            /**
             * @brief Helper function for move() to delegate castling case to
             * @param current_player The player which is castling
             * @param other_player The opponent which pieces might attack cells in between enemy king and rook
             * @param movement Movement with is_short_castling | is_long_castling flag == true
             * @return Chess::utilities::MoveResult invalid | ok
             */

            Chess::utilities::MoveResult handle_castling(Player& current_player, Player& other_player, Movement movement);
            /**
             * @brief Helper function for move() to delegate en passant to
             * @param current_player The player who is doing en passant
             * @param other_player The opponent who had just moved a piece of two in front next to an enemy pawn
             * @param movement Movement with is_en_passant flag == true
             */
            void handle_en_passant(Player& current_player, Player& other_player, Movement movement);

            std::map<std::string, int> position_history; //Map of number of occurence of positions during the game, for threefold repetition draw condition

            /**
             * @brief Takes snapshot of current board position, to be inserted in position_history
             * @param current_color Color of the current player, for determining who moves the next turn
             * @return std::string A string in fen format
             */
            std::string to_fen(Chess::utilities::Color current_color);

        public:

            /**
             * @brief Construct a new Board object
             * @param fen String in fen format
             * @param player1 Player to which the pieces are assigned based on the color
             * @param player2 Player to which the pieces are assigned based on the color
             */
            Board(std::string fen, Player* player1, Player* player2);

            /**
             * @brief Moves a piece according to parameter
             * @param current_player Player to which piece belongs to
             * @param other_player Opponent to which current_player might capture piece of
             * @param movement Movement
             * @return Chess::utilities::MoveResult invalid | ok | check | promotion
             */
            Chess::utilities::MoveResult move(Player& current_player, Player& other_player, Movement movement);

            /**
             * @brief To be used when last_movement has is_promotion flag == true
             * @param player Player whose pawn has reached opposite rank
             * @param piece_symbol Symbol in english standard of the piece you want the pawn to be promoted to
             */
            void promote(Player& player, char piece_symbol);

            /**
             * @brief Checks if the situation is checkmate
             * @param current Player you want to check checkmate for
             * @param other Opponent whose pieces might put current's king in checkmate
             * @return true 
             * @return false 
             */
            bool is_checkmate(Player& current, Player& other);

            /**
             * @brief Checks if the situation is draw
             * checks for, in order: threefold repetition, 50 moves rule, dead position and stalemate
             * @param current Player you want to check draw for
             * @param other Opponent whose pieces might put current's king in stalemate
             * @return true 
             * @return false 
             */
            bool is_draw(Player& current, Player& other); 

            Movement get_last_movement() const { return last_movement; } //Used by Pawn for checking for en passant conditions

            /**
             * @brief Get the piece at coordinate specified
             * @param coordinate Must be valid
             * @return Piece* copy of the pointer in the position
             */
            Piece* get_piece_at(Coordinate coordinate) const;

            /**
             * @brief Representation of current cells situation with ANSI formatting, for printing on a terminal 
             * @return std::string 
             */
            std::string pretty_print();

            /**
             * @brief Operator overloading. Representation of current cells situation, for printing on a file
             * @param os 
             * @param board
             * @return std::ostream& 
             */
            friend std::ostream& operator<<(std::ostream& os, const Board& board);

    }; //end class Board

} //end namespace Chess

#endif
