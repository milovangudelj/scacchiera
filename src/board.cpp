#include "Board.h"

#include <list>
#include <algorithm>

using Chess::Board;
using Chess::Piece;
using Chess::King;
using Chess::Queen;
using Chess::Bishop;
using Chess::Knight;
using Chess::Rook;
using Chess::Coordinate;
using Chess::Player;
using Chess::Movement;
using Chess::utilities::PieceType;
using Chess::utilities::Color;
using Chess::utilities::MoveResult;
using Chess::utilities::DirectionOffset;
using Chess::utilities::Direction;

//helper data or functions
std::map<char, Chess::utilities::PieceType> char_to_piece {
    {'k', Chess::utilities::PieceType::king},
    {'q', Chess::utilities::PieceType::queen},
    {'b', Chess::utilities::PieceType::bishop},
    {'n', Chess::utilities::PieceType::knight},
    {'r', Chess::utilities::PieceType::rook},
    {'p', Chess::utilities::PieceType::pawn}
};

std::shared_ptr<Piece> make_piece(Coordinate coordinate, Color color, PieceType type) {
    switch (type) {
        case PieceType::king :
            return std::make_shared<King>(coordinate, color, type);
            break;
        case PieceType::queen :
            return std::make_shared<Queen>(coordinate, color, type);
            break;
        case PieceType::bishop :
            return std::make_shared<Bishop>(coordinate, color, type);
            break;
        case PieceType::knight :
            return std::make_shared<Knight>(coordinate, color, type);
            break;
        case PieceType::rook :
            return std::make_shared<Rook>(coordinate, color, type);
            break;
        //TODO Pawn
    }
    return nullptr;
}
//end helper

Board::Board(std::string fen) {
    initialize_with_fen(fen);
}

void Board::initialize_with_fen(std::string fen) {
    unsigned int file = 0, rank = 0;
    for(char character : fen) {
        if(character == '/') {
            rank++;
            file = 0;
            continue;
        }
        if(std::isdigit(character)) {
            file += character;
            continue;
        }
        Color color = std::islower(character) ? Color::black : Color::white;
        PieceType type = char_to_piece.at(std::tolower(character));
        cells[rank][file] = make_piece({rank, file}, color, type);

        if(type == PieceType::king) {
            if(color == Color::white) {
                w_king_coordinate = {rank, file};
            } else {
                b_king_coordinate = {rank, file};
            }
        }
        file++;
    }
}

std::shared_ptr<Piece> Board::get_piece_at(Coordinate coordinate) {
    auto [rank, file] = coordinate;
    return cells[rank][file];
}

bool Board::is_check(Player& current, Player& other, Board& board) {
    Coordinate king_coordinate = (current.get_color() == Color::black) ? b_king_coordinate : w_king_coordinate;
    for(std::shared_ptr<Piece> piece : other.get_available_pieces()) {
        std::list<Movement> pseudo_movements = piece->get_pseudo_valid_movements(board);
        auto p = std::find_if(pseudo_movements.begin(), pseudo_movements.end(), 
            [king_coordinate](Movement movement) {
                return king_coordinate == movement.end;
            });
        if(p != pseudo_movements.end()) {
            return true;
        }
    }
    return false;
}

MoveResult Board::move(Player& current_player, Player& other_player, Movement movement) {
    //check if piece is owned by the player
    Coordinate start_coordinate = movement.start;
    auto [start_coordinate_rank, start_coordinate_file] = start_coordinate;
    std::shared_ptr<Piece> start_piece = cells[start_coordinate_rank][start_coordinate_file];
    if(start_piece->get_color() != current_player.get_color()) {
        return MoveResult::invalid;
    }

    //check if ending position is contained in pseudo_valid_movements of piece
    Coordinate end_coordinate = movement.end;
    auto [end_coordinate_rank, end_coordinate_file] = end_coordinate;
    std::shared_ptr<Piece> end_piece = cells[end_coordinate_rank][end_coordinate_file];
    std::list<Movement> pseudo_valid_movements = start_piece->get_pseudo_valid_movements(*this);
    auto p = std::find_if(pseudo_valid_movements.begin(), pseudo_valid_movements.end(), 
        [&] (Movement pseudo_movement) {
            Coordinate pseudo_coordinate = movement.end;
            auto [pseudo_coordinate_rank, pseudo_coordinate_file] = pseudo_coordinate;
            std::shared_ptr<Piece> test_piece = cells[pseudo_coordinate_rank][pseudo_coordinate_file];
            return *test_piece == *end_piece;
        }
    );
    if(p == pseudo_valid_movements.end()) {
        return MoveResult::invalid;
    }

    if(movement.is_short_castling || movement.is_long_castling) {
        return handle_castling(current_player, other_player, movement);
    }

    Movement previous_movement = last_movement;
    std::shared_ptr<Piece> previous_eaten = last_eaten;
    temporary_move(movement);
    if(is_check(current_player, other_player, *this)) {
        undo(previous_movement, previous_eaten);
        return MoveResult::invalid;
    } else {
        other_player.add_to_lost_pieces(other_player.remove_from_available_pieces(last_eaten));
    }
   
    if(movement.is_promotion) {
        return MoveResult::promotion;
    }
}

bool Board::promote(Player& player, char piece_symbol) {
    std::list<std::shared_ptr<Piece>> lost_pieces = player.get_lost_pieces();
    auto p = std::find_if(lost_pieces.begin(), lost_pieces.end(), 
        [piece_symbol] (std::shared_ptr<Piece> piece) {
            return piece_symbol == piece->get_symbol();
        }
    );
    if(p == lost_pieces.end()) {
        return false;
    }

    std::shared_ptr<Piece> resurrected = player.remove_from_lost_pieces(*p);
    player.add_to_available_pieces(resurrected);

    unsigned int promotion_rank = player.get_color() == Color::black ? 0 : 7;
    for(std::shared_ptr<Piece>& piece : cells.at(promotion_rank)) {
        if(piece->get_type() == PieceType::pawn) {
            resurrected->set_coordinate(piece->get_coordinate());
            auto [rank, file] = piece->get_coordinate();
            cells[rank][file] = resurrected;
            player.add_to_lost_pieces(player.remove_from_available_pieces(piece));
            break;
        }
    }
    return true;
}

void Board::temporary_move(Movement movement) {
    last_movement = movement;
    Coordinate start_coordinate = movement.start;
    auto [start_coordinate_rank, start_coordinate_file] = start_coordinate;
    Coordinate end_coordinate = movement.end;
    auto [end_coordinate_rank, end_coordinate_file] = end_coordinate;
    last_eaten = cells[end_coordinate_rank][end_coordinate_file];
    cells[end_coordinate_rank][end_coordinate_file] = cells[start_coordinate_rank][start_coordinate_file];
    cells[start_coordinate_rank][start_coordinate_file] = nullptr;
}

void Board::undo(Movement previous_movement, std::shared_ptr<Piece> previous_eaten) {
    auto [start_rank, start_file] = last_movement.start;
    auto [end_rank, end_file] = last_movement.end;
    cells[start_rank][start_file] = cells[end_rank][end_file];
    cells[end_rank][end_file] = last_eaten;
    last_eaten = previous_eaten;
    last_movement = previous_movement;
}

MoveResult Board::handle_castling(Player& current_player, Player& other_player, Movement movement) {
    Coordinate king_coordinate = current_player.get_color() == Color::black ? b_king_coordinate : w_king_coordinate;
    Coordinate current_king_coordinate = king_coordinate;
    //short castling(king or knight)
    if(movement.end == movement.start + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right) ||
        movement.end == movement.start + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left)) {
        Coordinate to_right;
        for(int i = 0; i < 2; i++) {
            to_right = current_king_coordinate + DirectionOffset.at(Direction::right);
            Movement previous_movement = last_movement;
            std::shared_ptr<Piece> previous_eaten = last_eaten;
            temporary_move({current_king_coordinate, to_right});
            if(is_check(current_player, other_player, *this)) {
                undo(previous_movement, previous_eaten);
                return MoveResult::invalid;
            }
            current_king_coordinate = to_right;
        }
        to_right = current_king_coordinate + DirectionOffset.at(Direction::right); //coordinates of rook
        auto [king_rank, king_file] = king_coordinate;
        cells[king_rank][king_file] = cells[to_right.rank][to_right.file];
        temporary_move({current_king_coordinate, to_right});
    } else { //long castling
        Coordinate to_left;
        for(int i = 0; i < 3; i++) {
            to_left = current_king_coordinate + DirectionOffset.at(Direction::left);
            Movement previous_movement = last_movement;
            std::shared_ptr<Piece> previous_eaten = last_eaten;
            temporary_move({current_king_coordinate, to_left});
            if(is_check(current_player, other_player, *this)) {
                undo(previous_movement, previous_eaten);
                return MoveResult::invalid;
            }
            current_king_coordinate = to_left;
        }
        to_left = current_king_coordinate + DirectionOffset.at(Direction::left); //coordinates of rook
        auto [king_rank, king_file] = king_coordinate;
        cells[king_rank][king_file] = cells[to_left.rank][to_left.file];
        temporary_move({current_king_coordinate, to_left});
    }
    return MoveResult::ok;
}


namespace Chess {
    std::ostream& operator<< (std::ostream& os, const Board& board) {
        std::shared_ptr<Piece> piece;
        for(int rank = 0; rank < SIZE; rank++) {
            //algebric notation is reversed from internal matrix representation
            os << 8 - rank << " ";
            for(int file = 0; file < SIZE; file++) {
                piece = board.cells[rank][file];
                if(piece == nullptr) {
                    os << " ";
                } else {
                    os << piece->get_symbol();
                }
            }
            os << "\n";
        }
        os << "\n" << "  ";
        for(int i = 0; i < SIZE; i++) {
            os << static_cast<char>('A' + i);
        }
        return os;
    }
}
