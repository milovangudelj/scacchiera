/**
 * @file knight.cpp
 * @author Yihui Zhu
 * @brief Definition file of Knight.h of Knight class
 */

#include "pieces/Knight.h"
#include "Board.h"

#include <array>

using Chess::Knight;

using Chess::Movement;
using Chess::utilities::Color;
using Chess::utilities::PieceType;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;

Knight::Knight(Coordinate coordinate, Color color, PieceType type) : Piece(coordinate, color, type) {
    symbol = (color == Color::black) ? "\u265E" : "\u2658";
}

std::list<Movement> Knight::get_pseudo_valid_movements(const Board& board) {
    std::list<Movement> pseudo_movements;
    
    std::array<Coordinate, 8> possible_movements;

    possible_movements[0] = this->coordinate + 
                            DirectionOffset.at(Direction::up) + 
                            DirectionOffset.at(Direction::left) + 
                            DirectionOffset.at(Direction::left);    //↑ ← ←
    possible_movements[1] = this->coordinate + 
                            DirectionOffset.at(Direction::up) + 
                            DirectionOffset.at(Direction::right) + 
                            DirectionOffset.at(Direction::right);   //↑ → →
    possible_movements[2] = this->coordinate +
                            DirectionOffset.at(Direction::up) + 
                            DirectionOffset.at(Direction::up) + 
                            DirectionOffset.at(Direction::left);    //↑ ↑ ←
    possible_movements[3] = this->coordinate +
                            DirectionOffset.at(Direction::up) +
                            DirectionOffset.at(Direction::up) + 
                            DirectionOffset.at(Direction::right);   //↑ ↑ →
    possible_movements[4] = this->coordinate + 
                            DirectionOffset.at(Direction::down) + 
                            DirectionOffset.at(Direction::left) + 
                            DirectionOffset.at(Direction::left);    //↓ ← ←
    possible_movements[5] = this->coordinate + 
                            DirectionOffset.at(Direction::down) + 
                            DirectionOffset.at(Direction::right) + 
                            DirectionOffset.at(Direction::right);   //↓ → →
    possible_movements[6] = this->coordinate +
                            DirectionOffset.at(Direction::down) + 
                            DirectionOffset.at(Direction::down) + 
                            DirectionOffset.at(Direction::left);    //↓ ↓ ←
    possible_movements[7] = this->coordinate +
                            DirectionOffset.at(Direction::down) +
                            DirectionOffset.at(Direction::down) + 
                            DirectionOffset.at(Direction::right);   //↓ ↓ →

    Piece *test_piece;
    for(Coordinate possible_move : possible_movements) {
        if(possible_move.is_valid()) {
            test_piece = board.get_piece_at(possible_move);
            if(test_piece == nullptr || test_piece->get_color() != this->color) {
                pseudo_movements.push_back({this->coordinate, possible_move});
            }
        }
    }

    return pseudo_movements;
}