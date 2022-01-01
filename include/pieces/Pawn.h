#ifndef pawn_h
#define pawn_h

#include "Piece.h"
#include<vector>
namespace Chess
{
    class PawnPiece : public Piece
    {
        using Piece::Piece;
        public:
        PawnPiece(Coordinate coordinate,Chess::utilities::Color color, Chess::utilities::PieceType type);
        std::list<Movement> get_pseudo_valid_movements(Board& board);
        private:
        Coordinate one_step_white(Board& board);
        Coordinate one_step_black(Board& board);
        std::vector<Coordinate> double_step_white(Board& board);
        std::vector<Coordinate> double_step_black(Board& board);
        std::vector<Coordinate> diag_cap_white(Board& board);
        std::vector<Coordinate> diag_cap_black(Board& board);
        bool promotion_white(Board& board);
        bool promotion_black(Board& board);
        std::vector<Coordinate> en_passant_white(Board& board);
        std::vector<Coordinate> en_passant_black(Board& board);
    };
}

#endif