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
        possible_movements = double_step_white(board);
        for(int i = 0; i < possible_movements.size(); i++)
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements[i],false,false,false});
        }
        //otherwise it can move only one square straight forward
        test_coordinate = one_step_white(board);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});    
        //unlike other pieces, a pawn captures in front of it diagonally
        possible_movements = diag_cap_white(board);
        for(int i = 0; i < possible_movements.size(); i++)
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements[i],false,false,false});
        }
        //promotion
        if(promotion_white(board))
            pseudo_moviments.push_back({this->coordinate,this->coordinate,true,false,false});
        //en passant's implementation
        possible_movements = en_passant_white(board);
        for(int i = 0; i < possible_movements.size(); i++)
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements[i],false,true,false});

        }
    }else
    {
        //double step straight forward black
        possible_movements = double_step_black(board);
        for(int i = 0; i < possible_movements.size(); i++)
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements[i],false,false,false});
        }
        //one step straight forward black
        test_coordinate = one_step_black(board);
        pseudo_moviments.push_back({this->coordinate,test_coordinate,false,false,false});
        //capture diagonally black
        possible_movements = diag_cap_black(board);
        for(int i = 0; i < possible_movements.size(); i++)
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements[i],false,false,false});

        }
        //promotion black
        if(promotion_black(board))
            pseudo_moviments.push_back({this->coordinate,this->coordinate,true,false,false});
        //en passant black
        possible_movements = en_passant_black(board);
        for(int i = 0; i < possible_movements.size(); i++)
        {
            pseudo_moviments.push_back({this->coordinate,possible_movements[i],false,true,false});
        }
    }
    return pseudo_moviments;
}
/************************** one step *****************************/
Chess::Coordinate PawnPiece::one_step_white(Board& board)
{
    Coordinate test_coordinate = this->coordinate + DirectionOffset.at(Direction::up);
    std::shared_ptr<Piece> test_piece;
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece == nullptr)
        {
            return test_coordinate;
        }
    }
    return test_coordinate;    
}

Chess::Coordinate PawnPiece::one_step_black(Board& board)
{
    Coordinate test_coordinate = this->coordinate + DirectionOffset.at(Direction::down);
    std::shared_ptr<Piece> test_piece;
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece == nullptr)
        {
            return test_coordinate;
        }
    }
    return test_coordinate;    
}
/************************** double step *****************************/
std::vector<Chess::Coordinate> PawnPiece::double_step_white(Board& board)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    std::vector<Coordinate> valid_coordinate;
    if(!get_had_moved())
    {
        int i = 0;
        test_coordinate = this->coordinate;
        do
        {
            test_coordinate = test_coordinate + DirectionOffset.at(Direction::up);
            if(test_coordinate.is_valid())
            {
                test_piece = board.get_piece_at(test_coordinate);
                if(test_piece == nullptr)
                {
                    valid_coordinate.push_back(test_coordinate);
                    return valid_coordinate;
                }
            }
            i++;
        } while (i<2);
    }
    return valid_coordinate;
}
std::vector<Chess::Coordinate> PawnPiece::double_step_black(Board& board)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    std::vector<Coordinate> valid_coordinate;
    if(!get_had_moved())
    {
        int i = 0;
        test_coordinate = this->coordinate;
        do
        {
            test_coordinate = test_coordinate + DirectionOffset.at(Direction::down);
            if(test_coordinate.is_valid())
            {
                test_piece = board.get_piece_at(test_coordinate);
                if(test_piece == nullptr)
                {
                    valid_coordinate.push_back(test_coordinate);
                }
            }
            i++;
        } while (i<2);
    }
    return valid_coordinate;
}
/************************** diagonally step *****************************/
std::vector<Chess::Coordinate> PawnPiece::diag_cap_white(Board& board)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    std::vector<Coordinate> valid_coordinate;
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
    return valid_coordinate;
}
std::vector<Chess::Coordinate> PawnPiece::diag_cap_black(Board& board)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    std::vector<Coordinate> valid_coordinate;
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
    return valid_coordinate;
}
/************************** en passant *****************************/
std::vector<Chess::Coordinate> PawnPiece::en_passant_white(Board& board)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    std::vector<Coordinate> valid_coordinate;
    test_coordinate = this->coordinate + DirectionOffset.at(Direction::left);
    Coordinate test_coordinate1 = test_coordinate + DirectionOffset.at(Direction::up) + DirectionOffset.at(Direction::up);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        std::list<Movement> list_of_movements = test_piece->get_pseudo_valid_movements(board);
        Movement last_move = list_of_movements.back();
        if(test_piece != nullptr && this->color != test_piece->get_color())
        {
            if(this->type == test_piece->get_type() && last_move.start == test_coordinate1)
            {
                test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_up);
                if(test_coordinate.is_valid())
                {
                    valid_coordinate.push_back(test_coordinate);
                }
            }
        }
    }
    test_coordinate = this->coordinate + DirectionOffset.at(Direction::right);
    Coordinate test_coordinate1 = test_coordinate + DirectionOffset.at(Direction::up) + DirectionOffset.at(Direction::up);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        std::list<Movement> list_of_movements = test_piece->get_pseudo_valid_movements(board);
        Movement last_move = list_of_movements.back();
        if(test_piece != nullptr && this->color != test_piece->get_color())
        {
            if(this->type == test_piece->get_type() && last_move.start == test_coordinate1)
            {
                test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_up);
                if(test_coordinate.is_valid())
                {
                    valid_coordinate.push_back(test_coordinate);
                }
            }
        }
    }
}
std::vector<Chess::Coordinate> PawnPiece::en_passant_black(Board& board)
{
    Coordinate test_coordinate;
    std::shared_ptr<Piece> test_piece;
    std::vector<Coordinate> valid_coordinate;
    test_coordinate = this->coordinate + DirectionOffset.at(Direction::left);
    Coordinate test_coordinate1 = test_coordinate + DirectionOffset.at(Direction::down) + DirectionOffset.at(Direction::down);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        std::list<Movement> list_of_movements = test_piece->get_pseudo_valid_movements(board);
        Movement last_move = list_of_movements.back();
        if(test_piece != nullptr && this->color != test_piece->get_color())
        {
            if(this->type == test_piece->get_type() && last_move.start == test_coordinate1)
            {
                test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_down);
                if(test_coordinate.is_valid())
                {
                    valid_coordinate.push_back(test_coordinate);
                }
            }
        }
    }
    test_coordinate = this->coordinate + DirectionOffset.at(Direction::right);
    Coordinate test_coordinate1 = test_coordinate + DirectionOffset.at(Direction::down) + DirectionOffset.at(Direction::down);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        std::list<Movement> list_of_movements = test_piece->get_pseudo_valid_movements(board);
        Movement last_move = list_of_movements.back();
        if(test_piece != nullptr && this->color != test_piece->get_color())
        {
            if(this->type == test_piece->get_type() && last_move.start == test_coordinate1)
            {
                test_coordinate = this->coordinate + DirectionOffset.at(Direction::right_down);
                if(test_coordinate.is_valid())
                {
                    valid_coordinate.push_back(test_coordinate);
                }
            }
        }
    }
}
/************************** promotion *****************************/
bool PawnPiece::promotion_white(Board& board)
{
    std::shared_ptr<Piece> test_piece = board.get_piece_at(this->coordinate);
    std::list<Movement> list_of_movement = test_piece->get_pseudo_valid_movements(board);
    Movement last_move = list_of_movement.back();
    if(last_move.end.rank == 0 )
    {
        return true;
    }
    return false;
}
bool PawnPiece::promotion_black(Board& board)
{
    std::shared_ptr<Piece> test_piece = board.get_piece_at(this->coordinate);
    std::list<Movement> list_of_movement = test_piece->get_pseudo_valid_movements(board);
    Movement last_move = list_of_movement.back();
    if(last_move.end.rank == 7 )
    {
        return true;
    }
    return false;
}
