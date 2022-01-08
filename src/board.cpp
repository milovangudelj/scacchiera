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
using Chess::Pawn;
using Chess::Coordinate;
using Chess::Player;
using Chess::Movement;
using Chess::utilities::PieceType;
using Chess::utilities::Color;
using Chess::utilities::MoveResult;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;

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
        case PieceType::queen :
            return std::make_shared<Queen>(coordinate, color, type);
        case PieceType::bishop :
            return std::make_shared<Bishop>(coordinate, color, type);
        case PieceType::knight :
            return std::make_shared<Knight>(coordinate, color, type);
        case PieceType::rook :
            return std::make_shared<Rook>(coordinate, color, type);
        case PieceType::pawn :
            return std::make_shared<Pawn>(coordinate, color, type);
    }
    return nullptr;
}
//end helper

Board::Board(std::string fen, Player& player1, Player& player2) : last_eaten{nullptr}, can_draw_flag{false} {
    initialize_with_fen(fen, player1, player2);
}

void Board::initialize_with_fen(std::string fen, Player& player1, Player& player2) {
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
        std::shared_ptr<Piece> piece = make_piece({rank, file}, color, type);
        cells[rank][file] = piece;
        if(player1.get_color() == color) {
            player1.add_to_available_pieces(piece);
        } else {
            player2.add_to_available_pieces(piece);
        }

        if(type == PieceType::king) {
            if(color == Color::white) {
                w_king_coordinate = {rank, file};
            } else {
                b_king_coordinate = {rank, file};
            }
        }
        if(type == PieceType::bishop) {
            std::static_pointer_cast<Bishop>(piece)->update_cell_color();
        }
        file++;
    }
}

std::shared_ptr<Piece> Board::get_piece_at(Coordinate coordinate) const {
    auto [rank, file] = coordinate;
    return cells[rank][file];
}

bool Board::is_check(Player& current, Player& other) {
    Coordinate king_coordinate = (current.get_color() == Color::black) ? b_king_coordinate : w_king_coordinate;
    for(std::shared_ptr<Piece> piece : other.get_available_pieces()) {
        std::list<Movement> pseudo_movements = piece->get_pseudo_valid_movements(*this);
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

bool Board::is_checkmate(Player& current, Player& other) {
    if(!is_check(current, other)) {
        return false;
    }
    auto king_iterator = std::find_if(current.get_available_pieces().begin(), current.get_available_pieces().end(), 
        [] (std::shared_ptr<Piece> piece) {
            return piece->get_type() == PieceType::king;
        }
    );
    std::shared_ptr<Piece> king = *king_iterator;
    Movement previous_movement = last_movement;
    std::shared_ptr<Piece> previous_eaten = last_eaten;
    for(Movement movement : king->get_pseudo_valid_movements(*this)) {
        temporary_move(movement);
        bool can_move = is_check(current, other) ? false : true;
        undo(previous_movement, previous_eaten);
        if(can_move) {
            return false;
        }
    }

    for(std::shared_ptr<Piece> piece : current.get_available_pieces()) {
        for(Movement movement : piece->get_pseudo_valid_movements(*this)) {
            temporary_move(movement);
            bool can_move = is_check(current, other) ? false : true;
            undo(previous_movement, previous_eaten);
            if(can_move) {
                return false;
            }
        }
    }
    return true;
}

//needs to be invoked twice both for current and for other
bool Board::is_draw(Player& current, Player& other) {
    Movement previous_movement = last_movement;
    std::shared_ptr<Piece> previous_eaten = last_eaten;

    //stalemate
    if(!is_check(current, other)) {
        for(std::shared_ptr<Piece> piece : current.get_available_pieces()) {
            for(Movement movement : piece->get_pseudo_valid_movements(*this)) {
                MoveResult result = move(current, other, movement);
                if(result != MoveResult::invalid) {
                    undo(previous_movement, previous_eaten);
                    return false;
                }
            }
        }
    }

    //dead position
    std::list<std::shared_ptr<Piece>> current_pieces = current.get_available_pieces();
    std::list<std::shared_ptr<Piece>> other_pieces = other.get_available_pieces();

    if(current_pieces.size() == other_pieces.size() == 1) {
        //only kings left
        return true;
    }

    if(current_pieces.size() == 2 && other_pieces.size() == 1) {
        //king + (bishop/knight) vs king
        std::shared_ptr<Piece> not_king = current_pieces.front()->get_type() == PieceType::king ? current_pieces.back() : current_pieces.front();
        if(not_king->get_type() == PieceType::bishop || not_king->get_type() == PieceType::knight) {
            return true;
        }
    }

    if(current_pieces.size() <= 3 && other_pieces.size() <= 3) {
        //king + 0-2 bishops vs king + 0-2 bishops all on cells of the same color
        std::list<std::shared_ptr<Piece>> bishops;
        current_pieces.splice(current_pieces.end(), other_pieces); //move other_pieces elements at the end of current_pieces
        for(std::shared_ptr<Piece> piece : current_pieces) {
            if(piece->get_type() != PieceType::king) {
                if(piece->get_type() == PieceType::bishop) {
                    bishops.push_back(piece);
                } else {
                    return false;
                }
            }
        }
        if(bishops.size() == 0) {
            return false;
        }
        Color cell_color = std::static_pointer_cast<Bishop>(bishops.front())->get_cell_color();
        for(std::shared_ptr<Piece> bishop : bishops) {
            if(std::static_pointer_cast<Bishop>(bishop)->get_cell_color() != cell_color) {
                return false;
            }
        }
        return true;
    }

    //50 move rule
    if(current.get_stale_since() >= 50 && other.get_stale_since() >= 50) {
        return true;
    }

    return false;
}

MoveResult Board::move(Player& current_player, Player& other_player, Movement movement) {
    //check if piece is owned by the player
    auto [start_coordinate_rank, start_coordinate_file] = movement.start;
    std::shared_ptr<Piece> start_piece = cells[start_coordinate_rank][start_coordinate_file];
    if(start_piece == nullptr) {
        return MoveResult::invalid;
    }
    if(start_piece->get_color() != current_player.get_color()) {
        return MoveResult::invalid;
    }

    //check if ending position is contained in pseudo_valid_movements of piece 
    std::list<Movement> pseudo_valid_movements = start_piece->get_pseudo_valid_movements(*this);
    auto p = std::find_if(pseudo_valid_movements.begin(), pseudo_valid_movements.end(), 
        [movement] (Movement pseudo_movement) {
            return pseudo_movement.end == movement.end;
        }
    );
    if(p == pseudo_valid_movements.end()) {
        return MoveResult::invalid;
    }

    if(movement.is_short_castling || movement.is_long_castling) {
        MoveResult castling_result = handle_castling(current_player, other_player, movement);
        if(castling_result == MoveResult::ok) {
            position_history[to_fen()]++;
        }
        return castling_result;
    }

    if(movement.is_en_passant) {
        handle_en_passant(current_player, other_player, movement);
        position_history[to_fen()]++;
        return MoveResult::ok;
    }

    //try to move
    Movement previous_movement = last_movement;
    std::shared_ptr<Piece> previous_eaten = last_eaten;
    temporary_move(movement);
    if(is_check(current_player, other_player)) {
        undo(previous_movement, previous_eaten);
        return MoveResult::invalid;
    }

    if(last_eaten != nullptr) {
        other_player.add_to_lost_pieces(other_player.remove_from_available_pieces(last_eaten));  
    }
    start_piece->set_had_moved();
    start_piece->set_coordinate(movement.end);
    if(start_piece->get_type() == PieceType::pawn || last_eaten == nullptr) {
        current_player.reset_stale_since();
    } else {
        current_player.increment_stale_since();
    }

    if(start_piece->get_type() == PieceType::king) {
        if(current_player.get_color() == Color::black) {
            b_king_coordinate = movement.end;
        } else {
            w_king_coordinate = movement.end;
        }
    }

    position_history[to_fen()]++;
   
    if(movement.is_promotion) {
        return MoveResult::promotion;
    } else {
        return MoveResult::ok;
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
    if(resurrected->get_type() == PieceType::bishop) {
        std::static_pointer_cast<Bishop>(resurrected)->update_cell_color();
    }
    player.add_to_available_pieces(resurrected);

    unsigned int promotion_rank = player.get_color() == Color::black ? 0 : 7;
    for(std::shared_ptr<Piece> piece : cells.at(promotion_rank)) {
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
    Coordinate& initial_king_coordinate = current_player.get_color() == Color::black ? b_king_coordinate : w_king_coordinate;
    Coordinate final_king_coordinate;
    Coordinate initial_rook_coordinate;
    Coordinate final_rook_coordinate;
    std::list<Coordinate> empty_cells;
    if(movement.is_short_castling) {
        empty_cells.push_back(initial_king_coordinate + DirectionOffset.at(Direction::right));
        empty_cells.push_back(initial_king_coordinate + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right));

        initial_rook_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right);
        final_rook_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::right);
        final_king_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right);
    } else {
        empty_cells.push_back(initial_king_coordinate + DirectionOffset.at(Direction::left));
        empty_cells.push_back(initial_king_coordinate + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left));
        empty_cells.push_back(initial_king_coordinate + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left));

        initial_rook_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left);
        final_rook_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::left);
        final_king_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left);
    }
    //check that cells in between king and rook aren't under attack
    for(std::shared_ptr<Piece> piece : other_player.get_available_pieces()) {
        for(Movement pseudo_movement : piece->get_pseudo_valid_movements(*this)) {            
            for(Coordinate empty_cell : empty_cells) {
                if(empty_cell == pseudo_movement.end) {
                    return MoveResult::invalid;
                }
            }
        }
    }

    Movement previous_movement = last_movement;
    std::shared_ptr<Piece> previous_eaten = last_eaten;
    Coordinate initial_king_coordinate_copy = initial_king_coordinate;

    std::shared_ptr<Piece> king = cells[initial_king_coordinate.rank][initial_king_coordinate.file];
    std::shared_ptr<Piece> rook = cells[initial_rook_coordinate.rank][initial_rook_coordinate.file];

    temporary_move({initial_king_coordinate, final_king_coordinate});
    initial_king_coordinate = final_king_coordinate;
    if(is_check(current_player, other_player)) {
        undo(previous_movement,previous_eaten);
        initial_king_coordinate = initial_king_coordinate_copy;
        return MoveResult::invalid;
    }
    temporary_move({initial_rook_coordinate, final_rook_coordinate});
    king->set_coordinate(final_king_coordinate);
    king->set_had_moved();
    rook->set_coordinate(final_rook_coordinate);
    rook->set_had_moved();
    current_player.increment_stale_since();
    return MoveResult::ok;
}

void Board::handle_en_passant(Player& current_player, Player& other_player, Movement movement) {
    Direction capture_direction;
    if(movement.end == movement.start + DirectionOffset.at(Direction::left_up) ||
       movement.end == movement.start + DirectionOffset.at(Direction::left_down)) { //en passant to left
        capture_direction = Direction::left;
    }  else {
        capture_direction = Direction::right;
    }
    temporary_move(movement);
    Coordinate capture_coordinate = movement.start + DirectionOffset.at(capture_direction);
    std::shared_ptr<Piece> captured_piece = get_piece_at(capture_coordinate);
    cells[capture_coordinate.rank][capture_coordinate.file] = nullptr;
    last_eaten = captured_piece;
    std::shared_ptr<Piece> pawn = cells[movement.end.rank][movement.end.file];
    pawn->get_had_moved();
    pawn->set_coordinate(movement.end);
    other_player.add_to_lost_pieces(other_player.remove_from_available_pieces(captured_piece));
    current_player.reset_stale_since();
}

std::string Board::to_fen() {
    std::string fen;
    int empty_count = 0;
    for(std::array<std::shared_ptr<Piece>, 8> rank : cells) {
        for(std::shared_ptr<Piece> piece : rank) {
            if(piece == nullptr) {
                empty_count++;
            } else {
                fen += empty_count + piece->get_symbol();
                empty_count = 0;
            }
        }
    }
    return fen;
}

bool Board::can_draw() {
    if(can_draw_flag) {
        //if it already can draw there is no need to check again
        return true;
    }
    auto p = std::find_if(position_history.begin(), position_history.end(), 
        [] (std::pair<std::string, int> position) {
            return position.second == 3;
        }
    );
    if(p == position_history.end()) {
        return false;
    }
    can_draw_flag = true;
    return true;
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
