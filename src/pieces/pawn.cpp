#include "pieces/Pawn.h"
#include "Board.h"

using Chess::PawnPiece;
using Chess::Movement;
using Chess::utilities::Color;
using Chess::utilities::PieceType;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;

PawnPiece::PawnPiece(Coordinate coordinate, Color color, PieceType type) : Piece{coordinate,color,type}
{
    symbol = (color == Color::black) ? 'P': 'p';
}
std::list<Movement> PawnPiece::get_pseudo_valid_movements(Board& board)
{
    std::list<Movement> pseudo_moviments;
    Coordinate test_coordinate;
    std::pair<Coordinate,Coordinate> possible_movements;
    if(this->color == Color::white)
    {
        //if it's the pawn's first move, it can move two squares straight forward
        test_coordinate = double_step(board, Color::white);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false,false});
        //otherwise it can move only one square straight forward
        test_coordinate = one_step(board,this->coordinate,Color::white);
        if(promotion(test_coordinate,Color::white))
        {
            pseudo_moviments.push_back({this->coordinate,test_coordinate,true,false,false,false});    
        }else
        {
            pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false,false});
        }
        //unlike other pieces, a pawn captures in front of it diagonally
        possible_movements = diag_cap(board,Color::white);
        if(promotion(possible_movements.first,Color::white))
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements.first,true,false,false,false});    
        }else
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements.first,false,false,false,false});
        }
        if(promotion(possible_movements.second,Color::white))
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements.second,true,false,false,false});    
        }else
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements.second,false,false,false,false});
        }
        //en passant's implementation
        test_coordinate = en_passant(board,Color::white);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,true,false,false});
    }else
    {
        //double step straight forward black
        test_coordinate = double_step(board,Color::black);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false,false});
        //one step straight forward black
        test_coordinate = one_step(board,this->coordinate,Color::black);
        if(promotion(test_coordinate,Color::black))
        {
            pseudo_moviments.push_back({this->coordinate,test_coordinate,true,false,false,false});    
        }else
        {
            pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false,false});
        }
        //capture diagonally black
        possible_movements = diag_cap(board,Color::black);
        if(promotion(possible_movements.first,Color::black))
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements.first,true,false,false,false});    
        }else
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements.first,false,false,false,false});
        }
        if(promotion(possible_movements.second,Color::black))
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements.second,true,false,false,false});    
        }else
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements.second,false,false,false,false});
        }
        //en passant black
        test_coordinate = en_passant(board,Color::black);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,true,false,false});
    }
    return pseudo_moviments;
}
/************************** one step *****************************/
Chess::Coordinate PawnPiece::one_step(Board& board,Coordinate test_coordinate, Color color)
{
    std::shared_ptr<Piece> test_piece;
    Direction direction = (color == Color::white ? Direction::up : Direction::down);
    test_coordinate = test_coordinate + DirectionOffset.at(direction);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece == nullptr)
        {
            return test_coordinate;
        }
    }
    return this->coordinate; 
}
/************************** double step *****************************/
Chess::Coordinate PawnPiece::double_step(Board& board,Color color)
{
    Coordinate test_coordinate = this->coordinate;
    std::shared_ptr<Piece> test_piece;
    if(!get_had_moved())
    {
        test_coordinate = one_step(board,one_step(board,test_coordinate,color),color);
        return test_coordinate;
    }
    return test_coordinate;
}

/************************** diagonally step *****************************/
std::pair<Chess::Coordinate,Chess::Coordinate> PawnPiece::diag_cap(Board& board, Color color)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    std::pair<Coordinate,Coordinate> valid_coordinate;
    Direction direction1 = (color == Color::white ? Direction::left_up : Direction::left_down);
    Direction direction2 = (color == Color::white ? Direction::right_up : Direction::right_down);
    test_coordinate = this->coordinate + DirectionOffset.at(direction1);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece != nullptr && this->color != test_piece->get_color() )
        {
            valid_coordinate.first = test_coordinate;
        }
    }
    test_coordinate = this->coordinate + DirectionOffset.at(direction2);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece != nullptr && this->color != test_piece->get_color() )
        {
            valid_coordinate.second = test_coordinate;
        }
    }
    return valid_coordinate;
}

/************************** en passant (!)*****************************/
Chess::Coordinate PawnPiece::en_passant(Board& board,Color color)
{
    Coordinate test_coordinate, test_coordinate1;
    std::shared_ptr<Piece> test_piece;
    std::list<Movement> list_of_movements;
    Movement last_move;
    if(color == Color::white)
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::left);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() && this->type == test_piece->get_type())
            {
                list_of_movements = test_piece->get_pseudo_valid_movements(board);
                last_move = list_of_movements.back();
                if(last_move.start.rank == 1)
                {
                    test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_up);
                    if(test_coordinate.is_valid())
                    {
                        return test_coordinate;
                    }
                }
            }
        }
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::right);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() && this->type == test_piece->get_type())
            {
                list_of_movements = test_piece->get_pseudo_valid_movements(board);
                last_move = list_of_movements.back();
                if(last_move.start.rank == 1)
                {
                    test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_up);
                    if(test_coordinate.is_valid())
                    {
                        return test_coordinate;
                    }
                }
            }
        }
    }else
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::left);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() && this->type == test_piece->get_type())
            {
                list_of_movements = test_piece->get_pseudo_valid_movements(board);
                last_move = list_of_movements.back();
                if(last_move.start.rank == 6)
                {
                    test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_down);
                    if(test_coordinate.is_valid())
                    {
                        return test_coordinate;
                    }
                }
            }
        }
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::right);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() && this->type == test_piece->get_type())
            {
                list_of_movements = test_piece->get_pseudo_valid_movements(board);
                last_move = list_of_movements.back();
                if(last_move.start.rank == 6)
                {
                    test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_down);
                    if(test_coordinate.is_valid())
                    {
                        return test_coordinate;
                    }
                }
            }
        }
    }
    return test_coordinate;
}

/************************** promotion *****************************/
bool PawnPiece::promotion(Coordinate coordinate,Color color)
{
    if(color == Color::white )
    {
        if(coordinate.rank == 0 )
        {
            return true;
        }
    }else
    {
        if(coordinate.rank == 7 )
        {
            return true;
        }
    }
    return false;
}