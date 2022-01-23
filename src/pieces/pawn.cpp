/**
 * @file pawn.cpp
 * @author Tommaso Jin
 * @brief  Definition file of Pawn.h of Pawn class
 */

#include "pieces/Pawn.h"
#include "Board.h"

using Chess::Movement;
using Chess::Pawn;
using Chess::utilities::Color;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;
using Chess::utilities::PieceType;

Pawn::Pawn(Coordinate coordinate, Color color, PieceType type) : Piece{coordinate, color, type}
{
    symbol = (color == Color::black) ? 'P' : 'p';
}

/// @brief to get the list of possible movements
/// @param board reference to board
/// @returns list<Movement>
std::list<Movement> Pawn::get_pseudo_valid_movements(const Board &board)
{
    std::list<Movement> pseudo_movements;
    Coordinate test_coordinate;
    std::pair<Coordinate, Coordinate> possible_movements;
    bool is_promo;
    // white pawn case
    if (this->color == Color::white)
    {
        // if it's the pawn's first move, it can move two squares straight forward
        test_coordinate = double_step(board, Color::white);
        if (!(test_coordinate == this->coordinate))
        {
            pseudo_movements.push_back({this->coordinate, test_coordinate});
        }

        // otherwise it can move only one square straight forward
        test_coordinate = one_step(board, this->coordinate, Color::white);
        if (!(test_coordinate == this->coordinate))
        {
            is_promo = promotion(test_coordinate, Color::white) ? true : false;
            pseudo_movements.push_back({this->coordinate, test_coordinate, is_promo});
        }

        // unlike other pieces, a pawn captures in front of it diagonally
        possible_movements = diag_cap(board, Color::white);
        if (!(possible_movements.first == Coordinate{9, 9}))
        {
            is_promo = promotion(possible_movements.first, Color::white) ? true : false;
            pseudo_movements.push_back({this->coordinate, possible_movements.first, is_promo});
        }
        if (!(possible_movements.second == Coordinate{9, 9}))
        {
            is_promo = promotion(possible_movements.second, Color::white) ? true : false;
            pseudo_movements.push_back({this->coordinate, possible_movements.second, is_promo});
        }

        // en passant
        test_coordinate = en_passant(board, Color::white);
        if (!(test_coordinate == this->coordinate))
        {
            pseudo_movements.push_back({this->coordinate, test_coordinate, false, true});
        }
    }
    else // black pawn case
    {
        // double step straight forward
        test_coordinate = double_step(board, Color::black);
        if (!(test_coordinate == this->coordinate))
        {
            pseudo_movements.push_back({this->coordinate, test_coordinate});
        }

        // one step straight forward
        test_coordinate = one_step(board, this->coordinate, Color::black);
        if (!(test_coordinate == this->coordinate))
        {
            is_promo = promotion(test_coordinate, Color::black) ? true : false;
            pseudo_movements.push_back({this->coordinate, test_coordinate, is_promo});
        }

        // capture diagonally
        possible_movements = diag_cap(board, Color::black);
        if (!(possible_movements.first == Coordinate{9, 9}))
        {
            is_promo = promotion(possible_movements.first, Color::black) ? true : false;
            pseudo_movements.push_back({this->coordinate, possible_movements.first, is_promo});
        }
        if (!(possible_movements.second == Coordinate{9, 9}))
        {
            is_promo = promotion(possible_movements.second, Color::black) ? true : false;
            pseudo_movements.push_back({this->coordinate, possible_movements.second, is_promo});
        }

        // en passant
        test_coordinate = en_passant(board, Color::black);
        if (!(test_coordinate == this->coordinate))
        {
            pseudo_movements.push_back({this->coordinate, test_coordinate, false, true});
        }
    }

    return pseudo_movements;
}

/************************** one step *****************************/
/// @brief to get the coordinate when pawn moves one square straight forward
/// @param board reference to board
/// @param test_coordinate pawn's start position
/// @param color pawn's color
/// @returns Coordinate
Chess::Coordinate Pawn::one_step(const Board &board, Coordinate test_coordinate, Color color)
{
    Piece *test_piece;
    Direction direction = (color == Color::white ? Direction::up : Direction::down);

    test_coordinate = test_coordinate + DirectionOffset.at(direction);

    if (test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);

        if (test_piece == nullptr)
        {
            return test_coordinate;
        }
    }

    return this->coordinate;
}

/************************** double step *****************************/
/// @brief to get the coordinate when pawn moves two square straight forward
/// @param board reference to board
/// @param color pawn's color
/// @returns Coordinate
Chess::Coordinate Pawn::double_step(const Board &board, Color color)
{
    Coordinate test_coordinate = this->coordinate;
    Piece *test_piece;

    if (!get_had_moved())
    {
        test_coordinate = one_step(board, one_step(board, test_coordinate, color), color);

        return test_coordinate;
    }

    return test_coordinate;
}

/************************** diagonally step *****************************/
/// @brief to get the coordinate's pair when pawn captures diagonally
/// @param board reference to board
/// @param color pawn's color
/// @returns pair<Coordinate,Coordinate>
std::pair<Chess::Coordinate, Chess::Coordinate> Pawn::diag_cap(const Board &board, Color color)
{
    Coordinate test_coordinate;
    Piece *test_piece;
    std::pair<Coordinate, Coordinate> valid_coordinate{{9, 9}, {9, 9}};
    Direction direction1 = (color == Color::white ? Direction::left_up : Direction::left_down);
    Direction direction2 = (color == Color::white ? Direction::right_up : Direction::right_down);

    test_coordinate = this->coordinate + DirectionOffset.at(direction1);

    if (test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);

        if (test_piece != nullptr && this->color != test_piece->get_color())
        {
            valid_coordinate.first = test_coordinate;
        }
    }

    test_coordinate = this->coordinate + DirectionOffset.at(direction2);

    if (test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);

        if (test_piece != nullptr && this->color != test_piece->get_color())
        {
            valid_coordinate.second = test_coordinate;
        }
    }

    return valid_coordinate;
}

/************************** en passant *****************************/
/// @brief to get the coordinate when pawn realizes en passant
/// @param board reference to board
/// @param color pawn's color
/// @returns Coordinate
Chess::Coordinate Pawn::en_passant(const Board &board, Color color)
{
    Coordinate end_p, start_p;
    Piece *test_piece;
    Direction direction = (color == Color::white ? Direction::up : Direction::down);
    Direction direction_l = (color == Color::white ? Direction::left_up : Direction::left_down);
    Direction direction_r = (color == Color::white ? Direction::right_up : Direction::right_down);

    end_p = this->coordinate + DirectionOffset.at(Direction::left);
    start_p = end_p + DirectionOffset.at(direction) + DirectionOffset.at(direction);

    if (end_p.is_valid() && start_p.is_valid())
    {
        test_piece = board.get_piece_at(end_p);

        if (test_piece != nullptr && this->color != test_piece->get_color() && this->type == test_piece->get_type())
        {
            if (board.get_last_movement().start == start_p && board.get_last_movement().end == end_p)
            {
                end_p = this->coordinate + DirectionOffset.at(direction_l);

                return end_p;
            }
        }
    }

    end_p = this->coordinate + DirectionOffset.at(Direction::right);
    start_p = end_p + DirectionOffset.at(direction) + DirectionOffset.at(direction);

    if (end_p.is_valid() && start_p.is_valid())
    {
        test_piece = board.get_piece_at(end_p);

        if (test_piece != nullptr && this->color != test_piece->get_color() && this->type == test_piece->get_type())
        {
            if (board.get_last_movement().start == start_p && board.get_last_movement().end == end_p)
            {
                end_p = this->coordinate + DirectionOffset.at(direction_r);

                return end_p;
            }
        }
    }

    return this->coordinate;
}

/************************** promotion *****************************/
/// @brief to get the condition to realize promotion
/// @param coordinate pawn's end position
/// @param color pawn's color
/// @returns bool
bool Pawn::promotion(Coordinate coordinate, Color color)
{
    if (color == Color::white)
    {
        if (coordinate.rank == 0)
        {
            return true;
        }
    }
    else
    {
        if (coordinate.rank == 7)
        {
            return true;
        }
    }

    return false;
}