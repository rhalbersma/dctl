#pragma once
#include "Driver.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

template<typename> class Position;

namespace successors {

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>
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
};

}       // namespace successors
}       // namespace dctl

// include template definitions inside header
#include "BothMoves.hpp"
