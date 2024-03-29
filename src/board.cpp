/**
 * @file board.cpp
 * @author Yihui Zhu
 * @brief Definition file of Board.h for Board class
 */

#include "Board.h"

#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <regex>
#include <stdexcept>
#include <algorithm>

using Chess::Bishop;
using Chess::Board;
using Chess::Coordinate;
using Chess::Movement;
using Chess::Piece;
using Chess::Player;
using Chess::utilities::BRIGHT;
using Chess::utilities::Color;
using Chess::utilities::Direction;
using Chess::utilities::DirectionOffset;
using Chess::utilities::GREEN_BG;
using Chess::utilities::GREEN_FG;
using Chess::utilities::INVERT;
using Chess::utilities::MoveResult;
using Chess::utilities::PieceType;
using Chess::utilities::RED_BG;
using Chess::utilities::RED_FG;
using Chess::utilities::RESET;

/**
 * @brief Converts from fen notation of piece to PieceType
 */
std::map<char, Chess::utilities::PieceType> char_to_piece {
	{'k', Chess::utilities::PieceType::king},
	{'q', Chess::utilities::PieceType::queen},
	{'b', Chess::utilities::PieceType::bishop},
	{'n', Chess::utilities::PieceType::knight},
	{'r', Chess::utilities::PieceType::rook},
	{'p', Chess::utilities::PieceType::pawn}
};

Board::Board(std::string fen, Player* player1, Player* player2) : selected_piece{nullptr}, last_eaten{nullptr}, cells{} {
	from_fen(fen, player1, player2);
}

std::vector<std::string> split(std::string target, char delimiter) { //Used for splitting fen into its fields
	std::stringstream ss(target);
	std::string piece;
	std::vector<std::string> pieces;

	while (std::getline(ss, piece, delimiter)) {
		pieces.push_back(piece);
	}
	return pieces;
}

void Board::from_fen(std::string fen, Player* player1, Player* player2)
{
	// Regex pattern to match a fen string
	std::string pattern = "^([1-8kqbnrpKQBNRP]{1,8}\\/){7}[1-8kqbnrpKQBNRP]{1,8}[\\s]{1}[wb]{1}[\\s]{1}(([-]{1})|[K?Q?k?q?]{1,4})[\\s]{1}([-]{1}|([a-h]{1}(3|6){1}))[\\s]{1}[0-9]+[\\s]{1}[0-9]+$";

	// Map of piece starting positions (not including pawns)
	std::string starting_pos = "rnbqkbnr";

	// Validate fen string against regex pattern
	if (!std::regex_match(fen, std::regex(pattern, std::regex::ECMAScript)))
	{
		std::cout << "Invalid FEN string. Make sure it matches this regex pattern:\n\n" << BRIGHT << pattern << RESET << "\n";
		exit(0);
	}

	// Splitting fen into its fields
	std::vector<std::string> fields(split(fen, ' '));

	// Splitting ranks from first field
	std::vector<std::string> ranks(split(fields[0], '/'));

	Color active_color;
	switch (fields[1][0]) {
		case 'w':
			active_color = Chess::utilities::Color::white;
		case 'b':
			active_color = Chess::utilities::Color::black;
	}

	std::string c_rights = fields[2];
	std::string ep_targets = fields[3];

	int half_clock = stoi(fields[4]);
	int full_moves = stoi(fields[5]);

	// Create pieces
	Color p_color;
	PieceType p_type;
	Coordinate p_pos;

	for (unsigned int i = 0; i < ranks.size(); i++) { // i will serve as the rank counter 
		std::string rank = ranks[i];
		unsigned int file = 0;

		for (char c : rank) {
			if (std::isdigit(c)) { // if it's a number advance file by that number 
				file += (c - '0');
				continue;
			}

			p_color = std::islower(c) ? Color::black : Color::white;
			p_type = char_to_piece.at(std::tolower(c));
			p_pos = {i, file};

			// Add piece to its player
			Piece* piece;
			if (player1->get_color() == p_color) {
				piece = player1->add_to_available_pieces({i, file}, p_color, p_type);
			} else {
				piece = player2->add_to_available_pieces({i, file}, p_color, p_type);
			}
			cells[i][file] = piece;

			// Set kings' coordinates
			if (p_type == PieceType::king) {
				if (p_color == Color::white) {
					w_king = piece;
				} else {
					b_king = piece;
				}
			}

			// Set had_moved flags
			if (p_type == PieceType::pawn) {
				if ((p_color == Color::white && (i != 6)) || (p_color == Color::black && (i != 1))) {
					piece->set_had_moved();
				}
			} else {
				unsigned int test_rank = std::islower(c) ? 0 : 7;
				unsigned int test_file = starting_pos.find_first_of(std::tolower(c));

				Coordinate test_pos1 = {test_rank, test_file};

				// Update test file before reassigning it
				test_file = starting_pos.find_last_of(std::tolower(c));

				Coordinate test_pos2 = {test_rank, test_file};

				if (p_pos != test_pos1 && p_pos != test_pos2)
					piece->set_had_moved();
			}

			// Castling & had moved for rooks
			if (!(c_rights.compare("-") == 0) && p_type == PieceType::rook) {
				// Castling flags to check for based on color
				std::pair<char, char> c_flags = (p_color == Color::white) ? std::pair<char, char>{'K', 'Q'} : std::pair<char, char>{'k', 'q'};

				// If flag corresponding to the rook's side is missing set had_moved
				bool king_side_missing = c_rights.find(c_flags.first) == std::string::npos;
				bool queen_side_missing = c_rights.find(c_flags.second) == std::string::npos;
				bool is_queen_side = file == 0;
				bool is_king_side = file == 7;

				if ((king_side_missing && is_king_side) || (queen_side_missing && is_queen_side))
					piece->set_had_moved();
			}

			file++;
		}
	}
}

Piece* Board::get_piece_at(Coordinate coordinate) const {
	return cells[coordinate.rank][coordinate.file];
}

bool Board::is_check(const Player& current, const Player& other) {
	Coordinate king_coordinate = (current.get_color() == Color::black) ? b_king->get_coordinate() : w_king->get_coordinate();

	for (Piece* piece : other.get_available_pieces()) {	

		if(!(piece == get_piece_at(piece->get_coordinate()))) { //just been eaten by a temporary move of currnet player
			continue;
		}
		std::list<Movement> pseudo_movements = piece->get_pseudo_valid_movements(*this);

		auto p = std::find_if(pseudo_movements.begin(), pseudo_movements.end(),
			[king_coordinate](Movement movement) {
				return king_coordinate == movement.end;
		});
		if (p != pseudo_movements.end()) {
			return true;
		}
	}
	return false;
}

bool Board::is_checkmate(Player& current, Player& other) {
	if (!is_check(current, other)) {
		return false;
	}

	Piece* king = current.get_color() == Color::black ? b_king : w_king;
	Coordinate king_coordinate = king->get_coordinate(); //copy of king's coordinates for restoring it later
	Movement previous_movement = last_movement;
	Piece* previous_eaten = last_eaten;

	for (Movement movement : king->get_pseudo_valid_movements(*this)) { //checks first if king can move
		temporary_move(movement);
		king->set_coordinate(movement.end);
		bool can_move = is_check(current, other) ? false : true;
		undo(previous_movement, previous_eaten);
		if (can_move) {
			king->set_coordinate(king_coordinate);
			return false;
		}
		king->set_coordinate(king_coordinate);
	}

	if(current.get_available_pieces().size() == 1) { //king cannot move and is the only piece left
		return true;
	}

	for (Piece* piece : current.get_available_pieces()) {
		if(piece->get_type() == PieceType::king) continue;
		for (Movement movement : piece->get_pseudo_valid_movements(*this)) {
			temporary_move(movement);
			bool can_move = is_check(current, other) ? false : true;
			undo(previous_movement, previous_eaten);
			if (can_move) {
				return false;
			}
		}
	}
	return true;
}

bool Board::is_draw(Player& current, Player& other) {

	//threefold repetition
	auto p = std::find_if(position_history.begin(), position_history.end(),
		[] (std::pair<std::string, int> position) {
			return position.second == 3;
	});
	if (p != position_history.end()) {
		return true;
	}

	//50 move rule
	if (current.get_stale_since() >= 50 && other.get_stale_since() >= 50) {
		return true;
	}

	//dead position
	std::list<Piece*> current_pieces = current.get_available_pieces();
	std::list<Piece*> other_pieces = other.get_available_pieces();

	if ((current_pieces.size() == 1) && (other_pieces.size() == 1)) { //only kings left
		return true;
	}

	if (current_pieces.size() == 2 && other_pieces.size() == 1) { //king + (bishop/knight) vs king
		Piece* not_king = current_pieces.front()->get_type() == PieceType::king ? current_pieces.back() : current_pieces.front();
		if (not_king->get_type() == PieceType::bishop || not_king->get_type() == PieceType::knight) {
			return true;
		}
	}

	if (current_pieces.size() == 1 && other_pieces.size() == 2) { //king vs king + (bishop/knight)	
		Piece* not_king = other_pieces.front()->get_type() == PieceType::king ? other_pieces.back() : other_pieces.front();
		if (not_king->get_type() == PieceType::bishop || not_king->get_type() == PieceType::knight) {
			return true;
		}
	}

	if (current_pieces.size() <= 3 && other_pieces.size() <= 3) { //king + 0-2 bishops vs king + 0-2 bishops all on cells of the same color		
		bool all_bishop = true;
		std::list<Piece*> bishops;
		current_pieces.splice(current_pieces.end(), other_pieces); //move other_pieces elements at the end of current_pieces
		for (Piece* piece : current_pieces) {
			if (piece->get_type() != PieceType::king) {
				if (piece->get_type() == PieceType::bishop) {
					bishops.push_back(piece);
				} else {
					all_bishop = false;
					break;
				}
			}
		}
		if (all_bishop) {
			bool all_same = true;
			Color cell_color = static_cast<Bishop*>(bishops.front())->get_cell_color();
			for (Piece* bishop : bishops) {
				if (static_cast<Bishop *>(bishop)->get_cell_color() != cell_color) {
					all_same = false;
					break;
				}
			}
			if(all_same) {
				return true;
			}
		}
	}

	//stalemate
	Movement previous_movement = last_movement;
	Piece* previous_eaten = last_eaten;

	Piece* king = current.get_color() == Color::black ? b_king : w_king;
	Coordinate king_coordinate = king->get_coordinate();

	if (!is_check(current, other)) {
		for (Piece* piece : current.get_available_pieces()) {
			if(!(piece == get_piece_at(piece->get_coordinate()))) { //the pieces exists on the board effectively
				continue;
			}
			for (Movement movement : piece->get_pseudo_valid_movements(*this)) {
				if (piece->get_type() == PieceType::king) {
					king->set_coordinate(movement.end);
				}
				temporary_move(movement);
				bool can_move = is_check(current, other) ? false : true;
				undo(previous_movement, previous_eaten);
				king->set_coordinate(king_coordinate);
				if (can_move) {
					return false;
				}
			}
		}
		return true;
	}

	return false;
}

MoveResult Board::move(Player& current_player, Player& other_player, Movement movement) {

	//check if piece is owned by the player
	Piece* start_piece = cells[movement.start.rank][movement.start.file];
	if (start_piece == nullptr) {
		return MoveResult::invalid;
	}
	if (start_piece->get_color() != current_player.get_color()) {
		return MoveResult::invalid;
	}

	// selected piece update
	if (selected_piece != nullptr) {
		selected_piece->set_selected(false);
	}
	start_piece->set_selected();
	selected_piece = start_piece;

	//check if ending position is contained in pseudo_valid_movements of piece
	std::list<Movement> pseudo_valid_movements = start_piece->get_pseudo_valid_movements(*this);
	auto p = std::find_if(pseudo_valid_movements.begin(), pseudo_valid_movements.end(),
		[movement] (Movement pseudo_movement) {
			return pseudo_movement.end == movement.end;
		});
	if (p == pseudo_valid_movements.end()) {
		return MoveResult::invalid;
	}

	movement = *p; //copy flags

	//special moves
	if (movement.is_short_castling || movement.is_long_castling) {
		MoveResult castling_result = handle_castling(current_player, other_player, movement);
		if (castling_result == MoveResult::ok) {
			position_history[to_fen(current_player.get_color())]++;
		}
		return castling_result;
	}

	if (movement.is_en_passant) {
		handle_en_passant(current_player, other_player, movement);
		position_history[to_fen(current_player.get_color())]++;
		return MoveResult::ok;
	}

	//try to move
	Movement previous_movement = last_movement;
	Piece* previous_eaten = last_eaten;
	temporary_move(movement);

	Coordinate king_coordinate = current_player.get_color() == Color::black ? b_king->get_coordinate() : w_king->get_coordinate();
	Piece* king = current_player.get_color() == Color::black ? b_king : w_king;

	if(start_piece->get_type() == PieceType::king) {
		king->set_coordinate(movement.end);
	}
	if (is_check(current_player, other_player)) {
		undo(previous_movement, previous_eaten);
		king->set_coordinate(king_coordinate);
		return MoveResult::check;
	}

	if (last_eaten != nullptr) {
		other_player.move_to_lost_pieces(last_eaten);
	}

	//update moved piece
	start_piece->set_had_moved();
	start_piece->set_coordinate(movement.end);

	//update 50 moves counter
	if (start_piece->get_type() == PieceType::pawn || last_eaten == nullptr) {
		current_player.reset_stale_since();
	} else {
		current_player.increment_stale_since();
	}
	
	position_history[to_fen(current_player.get_color())]++; //update threefold repetition counter

	if (movement.is_promotion) {
		return MoveResult::promotion;
	} else {
		return MoveResult::ok;
	}
}

void Board::promote(Player& player, char piece_symbol) {
	unsigned int promotion_rank = player.get_color() == Color::black ? 7 : 0;
	Piece* pawn;
	for(Piece* piece : cells.at(promotion_rank)) { //finds pawn who is promoting
		if(piece != nullptr && piece->get_type() == PieceType::pawn) {
			pawn = piece;
			break;
		}
	}
	Piece* promoted = player.add_to_available_pieces(pawn->get_coordinate(), player.get_color(), char_to_piece.at(piece_symbol));
	promoted->set_selected();
	selected_piece = promoted;
	if(promoted->get_type() == PieceType::bishop) {
		static_cast<Bishop*>(promoted)->update_cell_color();
	}
	player.move_to_lost_pieces(pawn);
	cells[promoted->get_coordinate().rank][promoted->get_coordinate().file] = promoted;
}

void Board::temporary_move(Movement movement) {
	last_movement = movement;
	Coordinate start_coordinate = movement.start;
	Coordinate end_coordinate = movement.end;
	last_eaten = cells[end_coordinate.rank][end_coordinate.file];
	cells[end_coordinate.rank][end_coordinate.file] = cells[start_coordinate.rank][start_coordinate.file];
	cells[start_coordinate.rank][start_coordinate.file] = nullptr;
}

void Board::undo(Movement previous_movement, Piece* previous_eaten) {
	Coordinate start = last_movement.start;
	Coordinate end = last_movement.end;
	cells[start.rank][start.file] = cells[end.rank][end.file];
	cells[end.rank][end.file] = last_eaten;
	last_eaten = previous_eaten;
	last_movement = previous_movement;
}

MoveResult Board::handle_castling(Player& current_player, Player& other_player, Movement movement) {
	Coordinate initial_king_coordinate = current_player.get_color() == Color::black ? b_king->get_coordinate() : w_king->get_coordinate();
	Coordinate final_king_coordinate;
	Coordinate initial_rook_coordinate;
	Coordinate final_rook_coordinate;
	std::pair<Coordinate, Coordinate> empty_cells; //cells in between king and rook

	if (movement.is_short_castling) {
		empty_cells.first = (initial_king_coordinate + DirectionOffset.at(Direction::right));
		empty_cells.second = (initial_king_coordinate + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right));

		initial_rook_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right);
		final_rook_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::right);
		final_king_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::right) + DirectionOffset.at(Direction::right);
	} else {
		empty_cells.first = (initial_king_coordinate + DirectionOffset.at(Direction::left));
		empty_cells.second = (initial_king_coordinate + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left));

		initial_rook_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left);
		final_rook_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::left);
		final_king_coordinate = initial_king_coordinate + DirectionOffset.at(Direction::left) + DirectionOffset.at(Direction::left);
	}

	//check that cells in between king and rook aren't under attack
	for (Piece* piece : other_player.get_available_pieces()) {
		for (Movement pseudo_movement : piece->get_pseudo_valid_movements(*this)) {
			if (empty_cells.first == pseudo_movement.end || empty_cells.second == pseudo_movement.end) {
				return MoveResult::invalid;
			}
		}
	}

	Piece* king = current_player.get_color() == Color::black ? b_king : w_king;
	Piece* rook = cells[initial_rook_coordinate.rank][initial_rook_coordinate.file];

	temporary_move({initial_king_coordinate, final_king_coordinate});
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
	if (movement.end == movement.start + DirectionOffset.at(Direction::left_up) ||
		 movement.end == movement.start + DirectionOffset.at(Direction::left_down)) { //en passant to left
		capture_direction = Direction::left;
	} else {
		capture_direction = Direction::right;
	}
	temporary_move(movement);
	Coordinate capture_coordinate = movement.start + DirectionOffset.at(capture_direction);
	Piece *captured_piece = get_piece_at(capture_coordinate);
	cells[capture_coordinate.rank][capture_coordinate.file] = nullptr;
	last_eaten = captured_piece;
	Piece *pawn = cells[movement.end.rank][movement.end.file];
	pawn->get_had_moved();
	pawn->set_coordinate(movement.end);
	other_player.move_to_lost_pieces(captured_piece);
	current_player.reset_stale_since();
}

std::string Board::to_fen(Color current_color) {
	std::string fen;

	//pieces positions
	int empty_count = 0;
	for (std::array<Piece*, 8> rank : cells) {
		for (Piece* piece : rank) {
			if(piece == nullptr) {
				empty_count++;
			} else {
				if(empty_count != 0) {
					fen += std::to_string(empty_count) + piece->get_symbol();
				} else {
					fen += piece->get_symbol();
				}
				empty_count = 0;
			}
		}
		if(empty_count != 0) {
			fen += std::to_string(empty_count) + "/";
		} else {
			fen += "/";
		}
		empty_count = 0;
	}

	//next turn
	fen += (current_color == Color::black) ? " w " : " b ";

	//castling rights
	bool w_kingside_c = false, w_queenside_c = false;
	for(Movement movement : w_king->get_pseudo_valid_movements(*this)) {
		if(movement.is_long_castling) {
			w_queenside_c = true;
		}
		if(movement.is_short_castling) {
			w_kingside_c = true;
		}
	}
	bool b_kingside_c = false, b_queenside_c = false;
	for(Movement movement : b_king->get_pseudo_valid_movements(*this)) {
		if(movement.is_long_castling) {
			b_queenside_c = true;
		}
		if(movement.is_short_castling) {
			b_queenside_c = true;
		}
	}
	if(w_kingside_c) fen += "K";
	if(w_queenside_c) fen += "Q";
	if(b_kingside_c) fen += "k";
	if(b_queenside_c) fen += "q";
	if(!w_kingside_c && !w_queenside_c && !b_kingside_c && !b_queenside_c) fen += "-";

	//en passant position
	Piece* last_moved = cells[last_movement.end.rank][last_movement.end.file];
	if(last_moved != nullptr && last_moved->get_type() == PieceType::pawn) {
		if(last_movement.end == last_movement.start + std::pair<int, int>(2, 0) || last_movement.end == last_movement.start + std::pair<int, int>(-2, 0)) {
			char file = last_movement.end.file + 'a';
			if(current_color == Color::black) {
				fen += " " + std::string(1, file) + "3";
			} else {
				fen += " " + std::string(1, file) + "6";
			}
		}
	} else {
		fen += " -";
	}

	return fen;
}

std::string Board::pretty_print() {
	std::stringstream ss;

	Piece* piece;
	for (int rank = 0; rank < SIZE; rank++) {	
		ss << 8 - rank << " "; //algebric notation is reversed from internal matrix representation
		for (int file = 0; file < SIZE; file++) {
			piece = cells[rank][file];
			ss << ((rank + file) % 2 == 0 ? INVERT : RESET); // Cell color
			if (piece == nullptr) {
				ss << "    ";
			} else {
				ss << (piece->is_selected() ? BRIGHT + std::string(((rank + file) % 2 == 0 ? RED_BG : RED_FG)) : ""); // Cell color
				ss << " " << piece->get_symbol() << "  ";
				ss << RESET;
			}
		}
		ss << "\033[0m"; // Color reset
		ss << "\n";
	}
	ss << "\n"
		<< "  ";
	for (int i = 0; i < SIZE; i++) {
		ss << " " << static_cast<char>('A' + i) << "  ";
	}
	return ss.str();
}

namespace Chess {
	std::ostream &operator<<(std::ostream &os, const Board &board) {
		Piece* piece;
		for (int rank = 0; rank < SIZE; rank++) {
			os << 8 - rank << " "; //algebric notation is reversed from internal matrix representation
			for (int file = 0; file < SIZE; file++) {
				piece = board.cells[rank][file];
				if (piece == nullptr) {
					os << " ";
				} else {
					os << piece->get_symbol();
				}
			}
			os << "\n";
		}
		os << "\n"
			<< "  ";
		for (int i = 0; i < SIZE; i++) {
			os << static_cast<char>('A' + i);
		}
		return os;
	}
}
