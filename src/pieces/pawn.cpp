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
    std::list<Movement> pseudo_moviment;
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
            test_coordinate = test_coordinate + DirectionOffset.at(Direction::up);
            if(test_coordinate.is_valid())
            {
                test_piece = board.get_piece_at(test_coordinate);
                if(test_piece!=nullptr)
                {
                    pseudo_moviment.push_back({this->coordinate,test_coordinate,false,false,false});
                }
            }
            i--;
        } while (i>0);
    }
    //otherwise it can move only one square straight forward
    test_coordinate = this->coordinate + DirectionOffset.at(Direction::up);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece == nullptr)
        {
            pseudo_moviment.push_back({this->coordinate,test_coordinate,false,false,false});
        }
    }
    //unlike other pieces, a pawn captures in front of it diagonally
    test_coordinate = this->coordinate + DirectionOffset.at(Direction::left_up);
    Coordinate test_coordinate2 = this->coordinate + DirectionOffset.at(Direction::right_up);
    if(test_coordinate.is_valid())
    {
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece != nullptr && this->color != test_piece->get_color() )
        {
            pseudo_moviment.push_back({this->coordinate,test_coordinate,false,false,false});
        }
    }
    if(test_coordinate2.is_valid())
    {
        std::shared_ptr<Piece> test_piece2 = board.get_piece_at(test_coordinate2);
        if(test_piece2 != nullptr && this->color != test_piece2->get_color() )
        {
            pseudo_moviment.push_back({this->coordinate,test_coordinate2,false,false,false});
        }
    }
    //if a pawn arrives at the opposite side, can be promoted to another piece(every piece can be chosen)
    test_coordinate = this->coordinate + DirectionOffset.at(Direction::up) + DirectionOffset.at(Direction::up);
    if(!test_coordinate.is_valid())
    {
        test_coordinate = this->coordinate + DirectionOffset.at(Direction::up);
        test_piece = board.get_piece_at(test_coordinate);
        if(test_piece == nullptr)
        {
            pseudo_moviment.push_back({this->coordinate,test_coordinate,true,false,false});
        }
    }
    //missing en passant's implementation
}