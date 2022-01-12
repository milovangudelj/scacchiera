#include"pieces/Rook.h"
#include"Board.h"

using Chess::Rook;
using Chess::Movement;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;
using Chess::utilities::Color;
using Chess::utilities::PieceType;

Rook::Rook(Coordinate coordinate, Color color, PieceType type) : Piece{coordinate,color,type}
{
	symbol= (color == Color::black) ? 'T' : 't';
}

std::list<Movement> Rook::get_pseudo_valid_movements(Board& board)
{
    Coordinate test_coordinate;
	std::list<Movement> pseudo_movements;
	for(int i = 0; i < 4; i++)
	{
		std::shared_ptr<Piece> test_piece;
		Direction direction = static_cast<Direction>(i);
		std::pair<int,int> offset = DirectionOffset.at(direction);
		test_coordinate = this->coordinate + offset;
		while(test_coordinate.is_valid())
		{
			test_piece = board.get_piece_at(test_coordinate);
			if(test_piece == nullptr)
			{
				pseudo_movements.push_back({this->coordinate,test_coordinate});
			}
			else
			{
				Color test_piece_color = test_piece->get_color();
				if(color != test_piece_color)
				{
					pseudo_movements.push_back({this->coordinate,test_coordinate});
				}
				break;
			}
			test_coordinate = test_coordinate + offset;
		}
	}

    if(!get_had_moved())
    {        
        test_coordinate = short_castling(board,this->color);
        if(!(test_coordinate == this->coordinate))
        {
            pseudo_movements.push_back({this->coordinate,test_coordinate,false,false,false,true});
        }
        test_coordinate = long_castling(board,this->color);
        if(!(test_coordinate == this->coordinate))
        {
            pseudo_movements.push_back({this->coordinate,test_coordinate,false,false,true,false});
        }
    }
    return pseudo_movements;
}
/************************** short castling *****************************/
Chess::Coordinate Rook::short_castling(Board& board,Color color)
{
    int i = 2;
    std::shared_ptr<Piece> test_piece;
    Coordinate test_coordinate = this->coordinate;
    int rook_rank = (color == Color::white ? 7 : 0); 
    int rook_file = (color == Color::white ? 7 : 0);
    Direction direction = (color == Color::white ? Direction::left : Direction::right);
    //if it's white rook, position H1
    //if it's black rook, position A8 
    if(this->coordinate.rank == rook_rank && this->coordinate.file == rook_file)
    {
        do
        {
            test_coordinate = test_coordinate + DirectionOffset.at(direction);
            if(test_coordinate.is_valid())
            {
                test_piece = board.get_piece_at(test_coordinate);
                //condition : no pieces between king abd rook
                if(test_piece != nullptr)
                {
                    break;
                }
            }
            i--;
        }while(i>0);
        test_coordinate = test_coordinate + DirectionOffset.at(direction);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            //condition : king's first move
            if(test_piece->get_type() == PieceType::king && test_piece->get_had_moved()==false)
            {
                return test_coordinate;
            }
        }
    }   
    return test_coordinate;
}
/************************** long castling *****************************/
Chess::Coordinate Rook::long_castling(Board& board, Color color)
{
    int i = 3;
    std::shared_ptr<Piece> test_piece;
    Coordinate test_coordinate = this->coordinate;
    int rook_rank = (color == Color::white ? 7 : 0); 
    int rook_file = (color == Color::white ? 0 : 7);
    Direction direction = (color == Color::white ? Direction::right : Direction::left);
    //if it's white rook, position A1
    //if it's black rook, position H8
    if(this->coordinate.rank == rook_rank && this->coordinate.file == rook_file)
    {
        do
        {
            test_coordinate = test_coordinate + DirectionOffset.at(direction);
            if(test_coordinate.is_valid())
            {
                test_piece = board.get_piece_at(test_coordinate);
                //condition : no pieces between king abd rook
                if(test_piece != nullptr)
                {
                    break;
                }
            }
            i--;
        }while(i>0);
        test_coordinate = test_coordinate + DirectionOffset.at(direction);
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            //condition : king's first move
            if(test_piece->get_type() == PieceType::king && test_piece->get_had_moved()==false)
            {
                return test_coordinate;
            }
        }
    }   
    return test_coordinate;
}