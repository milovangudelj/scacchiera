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
using Chess::utilities::PieceType;
using Chess::utilities::Color;

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

namespace Chess {
    std::ostream& operator<< (std::ostream& os, const Board& board) {
        for(const auto& rank : board.cells) {
            for(const auto& file : rank) {
                os << file->get_symbol() << " ";
            }
            os << "\n";
        }
        return os;
    }
}
