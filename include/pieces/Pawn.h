/**
 * @file Pawn.h
 * @author Tommaso Jin
 * @brief  Header file of Pawn class
 */

#ifndef pawn_h
#define pawn_h

#include "Piece.h"
#include <utility>
namespace Chess
{
    class Pawn : public Piece
    {
        using Piece::Piece;

    public:
        Pawn(Coordinate coordinate, Chess::utilities::Color color, Chess::utilities::PieceType type);
        std::list<Movement> get_pseudo_valid_movements(Board &board);

    private:
        Coordinate one_step(Board &board, Coordinate coordinate, Chess::utilities::Color color);
        Coordinate double_step(Board &board, Chess::utilities::Color color);
        std::pair<Coordinate, Coordinate> diag_cap(Board &board, Chess::utilities::Color color);
        bool promotion(Coordinate coordinate, Chess::utilities::Color color);
        Coordinate en_passant(Board &board, Chess::utilities::Color color);
    };
}

#endif