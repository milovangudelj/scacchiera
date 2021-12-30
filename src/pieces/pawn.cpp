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
    std::pair<int,int> offset; 
    std::shared_ptr<Piece> test_piece;
    //if it's the pawn's first move, it can move two squares straight forward
    if(!get_had_moved())
    {
        int i = 2;
        test_coordinate = this->coordinate;
        do
        {
            if(this->color == Color::white)
            {
                test_coordinate = test_coordinate + DirectionOffset.at(Direction::up);
                if(test_coordinate.is_valid())
                {
                    test_piece = board.get_piece_at(test_coordinate);
                    if(test_piece!=nullptr)
                    {
                        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
                    }
                }
            }else
            {
                test_coordinate = test_coordinate + DirectionOffset.at(Direction::down);
                if(test_coordinate.is_valid())
                {
                    test_piece = board.get_piece_at(test_coordinate);
                    if(test_piece!=nullptr)
                    {
                        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
                    }
                }
            }
            i--;
        } while (i>0);
    }
    //otherwise it can move only one square straight forward
    if(this->color == Color::white)
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::up);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr)
            {
                pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
            }
        }
    }else
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::down);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr)
            {
                pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
            }
        }
    }
    
    //unlike other pieces, a pawn captures in front of it diagonally
    if(this->color == Color::white)
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_up);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() )
            {
                pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
            }
        }
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_up);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() )
            {
                pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
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
                pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
            }
        }
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_down);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece != nullptr && this->color != test_piece->get_color() )
            {
                pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
            }
        }
    }
    
    //if a pawn arrives at the opposite side, can be promoted to another piece(every piece can be chosen)
    if(this->color == Color::white)
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::up) + DirectionOffset.at(Direction::up);
        if(!test_coordinate.is_valid())
        {
            test_coordinate = this->coordinate + DirectionOffset.at(Direction::up);
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr)
            {
                pseudo_moviments.push_back({this->coordinate,test_coordinate,true,false,false});
            }
        }
    }else
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::down) + DirectionOffset.at(Direction::down);
        if(!test_coordinate.is_valid())
        {
            test_coordinate = this->coordinate + DirectionOffset.at(Direction::down);
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece == nullptr)
            {
                pseudo_moviments.push_back({this->coordinate,test_coordinate,true,false,false});
            }
        }
    }
    
    //missing en passant's implementation
    if(this->color == Color::white)
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::left);
        Coordinate test_coordinate1 = test_coordinate + DirectionOffset.at(Direction::up) + DirectionOffset.at(Direction::up);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            std::list<Movement> list_of_movement = test_piece->get_pseudo_valid_movements(board);
            Movement last_move = list_of_movement.back();
            Coordinate move = last_move.start;
            if(test_piece != nullptr && this->color != test_piece->get_color())
            {
                if(this->type == test_piece->get_type() && last_move.start == test_coordinate1)
                {
                    test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_up);
                    if(test_coordinate.is_valid())
                    {
                        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,true,false});
                    }
                }
            }
        }
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::right);
        Coordinate test_coordinate1 = test_coordinate + DirectionOffset.at(Direction::up) + DirectionOffset.at(Direction::up);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            std::list<Movement> list_of_movement = test_piece->get_pseudo_valid_movements(board);
            Movement last_move = list_of_movement.back();
            Coordinate move = last_move.start;
            if(test_piece != nullptr && this->color != test_piece->get_color())
            {
                if(this->type == test_piece->get_type() && last_move.start == test_coordinate1)
                {
                    test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_up);
                    if(test_coordinate.is_valid())
                    {
                        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,true,false});
                    }
                }
            }
        }
    }else
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::left);
        Coordinate test_coordinate1 = test_coordinate + DirectionOffset.at(Direction::down) + DirectionOffset.at(Direction::down);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            std::list<Movement> list_of_movement = test_piece->get_pseudo_valid_movements(board);
            Movement last_move = list_of_movement.back();
            Coordinate move = last_move.start;
            if(test_piece != nullptr && this->color != test_piece->get_color())
            {
                if(this->type == test_piece->get_type() && last_move.start == test_coordinate1)
                {
                    test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_down);
                    if(test_coordinate.is_valid())
                    {
                        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,true,false});
                    }
                }
            }
        }
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::right);
        Coordinate test_coordinate1 = test_coordinate + DirectionOffset.at(Direction::down) + DirectionOffset.at(Direction::down);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            std::list<Movement> list_of_movement = test_piece->get_pseudo_valid_movements(board);
            Movement last_move = list_of_movement.back();
            Coordinate move = last_move.start;
            if(test_piece != nullptr && this->color != test_piece->get_color())
            {
                if(this->type == test_piece->get_type() && last_move.start == test_coordinate1)
                {
                    test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_down);
                    if(test_coordinate.is_valid())
                    {
                        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,true,false});
                    }
                }
            }
        }
    }
    return pseudo_moviments;
}