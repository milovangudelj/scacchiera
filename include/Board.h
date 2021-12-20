#ifndef board_h
#define board_h

#include <array>

template<typename T, unsigned int SIZE> 
using Matrix = std::array<std::array<T, SIZE>, SIZE>;

namespace Chess {

    class Board {
        private:
            Matrix<std::shared_ptr<Piece>, 8> cells;
        public:
            std::shared_ptr<Piece> get_piece_at(Coordinate coordinate);
    };
}

#endif
