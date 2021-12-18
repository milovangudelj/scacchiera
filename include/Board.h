#ifndef board_h
#define board_h

namespace Chess {

    class Board {
        private:
            Piece* cells[8][8];
        public:
            Piece* get_piece_at(Coordinate coordinate);
    };
}

#endif
