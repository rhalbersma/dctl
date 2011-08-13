#pragma once
#include "Driver.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

template<typename> class Position;

namespace successors {

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Stack&);
        static void generate_reverse(const Position<Board>&, Stack&);
        static void generate_promotions(const Position<Board>&, Stack&);

        static int count(const Position<Board>&);
        static int count_reverse(const Position<Board>&);
        static int count_promotions(const Position<Board>&);
        
        static bool detect(const Position<Board>&);
        static bool detect_reverse(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

private:
        // implementation
        static void generate_dirs(BitBoard, BitBoard, Stack&);
        static void generate_reverse_dirs(BitBoard, BitBoard, Stack&);
        template<int> static void generate_dir(BitBoard, BitBoard, Stack&);

        static int count_dirs(BitBoard, BitBoard);
        static int count_reverse_dirs(BitBoard, BitBoard);
        template<int> static int count_dir(BitBoard, BitBoard);

        static bool detect_dirs(BitBoard, BitBoard);
        static bool detect_reverse_dirs(BitBoard, BitBoard);
        template<int> static bool detect_dir(BitBoard, BitBoard);

        static BitBoard promotion(BitBoard);
        static BitBoard promotors(BitBoard);
};

}       // namespace successors
}       // namespace dctl

// include template definitions inside header
#include "PawnMoves.hpp"
