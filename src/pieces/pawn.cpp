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
    std::vector<Coordinate> possible_movements;
    if(this->color == Color::white)
    {
        //if it's the pawn's first move, it can move two squares straight forward
        test_coordinate = double_step(board, Color::white);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
        //otherwise it can move only one square straight forward
        test_coordinate = one_step(board,this->coordinate,Color::white);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});    
        //unlike other pieces, a pawn captures in front of it diagonally
        possible_movements = diag_cap(board,Color::white);
        for(int i = 0; i < possible_movements.size(); i++)
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements[i],false,false,false});
        }
        //promotion
        if(promotion(board,Color::white))
            pseudo_moviments.push_back({this->coordinate,this->coordinate,true,false,false});
        //en passant's implementation
        test_coordinate = en_passant(board,Color::white);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,true,false});
    }else
    {
        //double step straight forward black
        test_coordinate = double_step(board,Color::black);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
        //one step straight forward black
        test_coordinate = one_step(board,this->coordinate,Color::black);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
        //capture diagonally black
        possible_movements = diag_cap(board,Color::black);
        for(int i = 0; i < possible_movements.size(); i++)
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements[i],false,false,false});

        }
        //promotion black
        if(promotion(board,Color::black))
            pseudo_moviments.push_back({this->coordinate,this->coordinate,true,false,false});
        //en passant black
        test_coordinate = en_passant(board,Color::black);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,true,false});
    }
    return pseudo_moviments;
}
/************************** one step *****************************/
Chess::Coordinate PawnPiece::one_step(Board& board,Coordinate test_coordinate, Color color)
{
    std::shared_ptr<Piece> test_piece;
    if(color == Color::white)
    {
        test_coordinate = test_coordinate + DirectionOffset.at(Direction::up);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr)
            {
                return test_coordinate;
            }
        }
    }else
    {
        test_coordinate = test_coordinate + DirectionOffset.at(Direction::down);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr)
            {
                return test_coordinate;
            }
        }
    }
    return test_coordinate; 
}
/************************** double step *****************************/
Chess::Coordinate PawnPiece::double_step(Board& board,Color color)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    if(!get_had_moved())
    {
        test_coordinate = one_step(board,one_step(board,this->coordinate,color),color);
        return test_coordinate;
    }
    return test_coordinate;
}

/************************** diagonally step *****************************/
std::vector<Chess::Coordinate> PawnPiece::diag_cap(Board& board, Color color)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    std::vector<Coordinate> valid_coordinate;
    if(color == Color::white)
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_up);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() )
            {
                valid_coordinate.push_back(test_coordinate);
            }
        }
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_up);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() )
            {
                valid_coordinate.push_back(test_coordinate);
            }
        }
    }else
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_down);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() )
            {
                valid_coordinate.push_back(test_coordinate);
            }
        }
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_down);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() )
            {
                valid_coordinate.push_back(test_coordinate);
            }
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
bool PawnPiece::promotion(Board& board,Color color)
{
    std::shared_ptr<Piece> test_piece = board.get_piece_at(this->coordinate);
    std::list<Movement> list_of_movement = test_piece->get_pseudo_valid_movements(board);
    Movement last_move = list_of_movement.back();
    if(color == Color::white)
    {
        if(last_move.end.rank == 0 )
        {
            return true;
        }

    }else
    {
        if(last_move.end.rank == 7 )
        {
            return true;
        }
    }
    return false;
}

