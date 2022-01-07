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

	std::list<Movement> pseudo_movements;
	for(int i = 0; i < 4; i++)
	{
		std::shared_ptr<Piece> test_piece;
		Direction direction = static_cast<Direction>(i);
		std::pair<int,int> offset = DirectionOffset.at(direction);
		Coordinate test_coordinate = this->coordinate + offset;
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
        int i = 2;
        std::shared_ptr<Piece> test_piece;
        Coordinate test_coordinate = this->coordinate;
        std::pair<int, int> offset;
        do
        {
            offset = DirectionOffset.at(Direction::left);
            test_coordinate = test_coordinate + offset;
            if(test_coordinate.is_valid())
            {
                test_piece = board.get_piece_at(test_coordinate);
                if(test_piece != nullptr)
                {
                    return pseudo_movements;
                }
            }
            i--;
        }while(i>0);
        offset = DirectionOffset.at(Direction::left);
        test_coordinate = test_coordinate + offset;
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece->get_type() == PieceType::king && test_piece->get_had_moved()==false)
            {
                offset = DirectionOffset.at(Direction::right);
                test_coordinate = test_coordinate + offset;
                pseudo_movements.push_back({this->coordinate,test_coordinate,false,false,false,true});
            }
        }
        i = 3;
        do
        {
            offset = DirectionOffset.at(Direction::right);
            test_coordinate = test_coordinate + offset;
            if(test_coordinate.is_valid())
            {
                test_piece = board.get_piece_at(test_coordinate);
                if(test_piece != nullptr)
                {
                    return pseudo_movements;
                }
            }
            i--;
        }while(i>0);
        offset = DirectionOffset.at(Direction::right);
        test_coordinate = test_coordinate + offset;
        if(test_coordinate.is_valid())
        {
            test_piece = board.get_piece_at(test_coordinate);
            if(test_piece->get_type() == PieceType::king && test_piece->get_had_moved()==false)
            {
                offset = DirectionOffset.at(Direction::left);
                test_coordinate = test_coordinate + offset;
                pseudo_movements.push_back({this->coordinate,test_coordinate,false,false,true,false});
            }
        }
    }
	return pseudo_movements;

}