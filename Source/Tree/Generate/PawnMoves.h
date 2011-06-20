#pragma once
#include "Driver.h"
#include "../Node/Stack.h"
#include "../../Utilities/IntegerTypes.h"

namespace tree {

namespace node { template<typename> class Position; }

namespace generate {

template<bool Color, typename Rules, typename Board> 
class Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>
{
public:
        static void generate(const node::Position<Board>&, node::Stack*);
        static void generate_reverse(const node::Position<Board>&, node::Stack*);
        static void generate_promotions(const node::Position<Board>&, node::Stack*);

        static size_t count(const node::Position<Board>&);
        static size_t count_reverse(const node::Position<Board>&);
        static size_t count_promotions(const node::Position<Board>&);
        
        static bool detect(const node::Position<Board>&);
        static bool detect_reverse(const node::Position<Board>&);
        static bool detect_promotions(const node::Position<Board>&);

private:
        // implementation
        static void generate_dirs(BitBoard, BitBoard, node::Stack*);
        static void generate_reverse_dirs(BitBoard, BitBoard, node::Stack*);
        template<size_t> static void generate_dir(BitBoard, BitBoard, node::Stack*);

        static size_t count_dirs(BitBoard, BitBoard);
        static size_t count_reverse_dirs(BitBoard, BitBoard);
        template<size_t> static size_t count_dir(BitBoard, BitBoard);

        static bool detect_dirs(BitBoard, BitBoard);
        static bool detect_reverse_dirs(BitBoard, BitBoard);
        template<size_t> static bool detect_dir(BitBoard, BitBoard);

        static BitBoard promotion(BitBoard);
        static BitBoard promotors(BitBoard);
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnMoves.hpp"
