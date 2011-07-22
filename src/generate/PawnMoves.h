#pragma once
#include "Driver.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

template<typename> class Position;

namespace generate {

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Pieces::PAWN, MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Stack&);
        static void generate_reverse(const Position<Board>&, Stack&);
        static void generate_promotions(const Position<Board>&, Stack&);

        static size_t count(const Position<Board>&);
        static size_t count_reverse(const Position<Board>&);
        static size_t count_promotions(const Position<Board>&);
        
        static bool detect(const Position<Board>&);
        static bool detect_reverse(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

private:
        // implementation
        static void generate_dirs(BitBoard, BitBoard, Stack&);
        static void generate_reverse_dirs(BitBoard, BitBoard, Stack&);
        template<size_t> static void generate_dir(BitBoard, BitBoard, Stack&);

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
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnMoves.hpp"
